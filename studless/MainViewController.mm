//
//  GameViewController.m
//  legoview
//
//  Created by Leonid on 8/3/15.
//  Copyright (c) 2015 lFroen. All rights reserved.
//

#import "MainViewController.h"
#import <OpenGLES/ES2/glext.h>

#import "app_bridge.h"

#import "LegoModelPartsReport.h"
#import "PartReportViewController.h"


@interface CustomGLKViewController () {
    ModelRenderToImage *mImageRender;
}
@end

@implementation CustomGLKViewController

@synthesize Renderer = mLdrawRenderer;
@synthesize Model = mModel;

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    [EAGLContext setCurrentContext:self.context];
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(receiveModelLoadNotification:)
                                                 name:@"ModelLoadNotification"
                                               object:nil];
    
    [self initBars];
    
    [self initLib];
    
    NSString *modelPath = [[NSUserDefaults standardUserDefaults] valueForKey:@"modelPath"];
    if ( modelPath != nil ) {
        [self loadModel:modelPath];
    } else {
        [self loadDefaultModel];
    }
    
    [self setDragModeRotate];
    [self setTitle:@"Model"];
}

- (void)viewWillAppear:(BOOL)animated
{
    [EAGLContext setCurrentContext:self.context];
    
    [super viewWillAppear:animated];
    [[self navigationController] setNavigationBarHidden:YES animated:YES];
    [[self navigationController] setToolbarHidden:NO];
}

- (void)viewDidAppear:(BOOL)animated
{
    CGRect r = [self.view bounds];
    
    mInputProcess->setAspect(r.size.width, r.size.height);
    mLdrawRenderer->setProjectMatrix(r.size.width, r.size.height);

    int disasemblyState = mModel->getDisassemblyState();
    
    m_StepSlider.maximumValue = mModel->PieceCount();
    if ( disasemblyState > 0 ) {
        m_StepSlider.hidden = FALSE;
        m_buttonRestoreNext.enabled = true;
        m_StepSlider.value = mModel->PieceCount() - disasemblyState;
    } else {
        m_StepSlider.hidden = TRUE;
        m_buttonRestoreNext.enabled = false;
//    mModel->resetDisassemblyState();
    }
    
    [super viewDidAppear:animated];
}

-(IBAction)handlePanGestureWithGestureRecognizer:(UIPanGestureRecognizer *)gestureRecognizer {
    CGPoint pt;
    switch (gestureRecognizer.state) {
        case UIGestureRecognizerStateBegan:
            pt = [gestureRecognizer locationOfTouch:0 inView:self.view];
            break;
        case UIGestureRecognizerStateChanged:
            pt = [gestureRecognizer translationInView:self.view];
            mInputProcess->dragByOffset(pt.x, pt.y);
            break;
        case UIGestureRecognizerStateEnded:
            mInputProcess->dragEnd();
            break;
        default:
            break;
    }
}

-(IBAction)handleTapGestureWithGestureRecognizer:(UITapGestureRecognizer *)gestureRecognizer {
//    NSString *ldrawXmlPath = [[NSBundle mainBundle] pathForResource:@"ldraw" ofType:@"xml"];
//    LdrawXmlParser *ldr = [LdrawXmlParser ldrawXml:ldrawXmlPath];
//    printf("done!\n");
}

-(IBAction)handlePinchGestureWithGestureRecognizer:(UIPinchGestureRecognizer *)gestureRecognizer {
    switch (gestureRecognizer.state) {
        case UIGestureRecognizerStateChanged:
            mInputProcess->pitchChangeBy(gestureRecognizer.scale);
            break;
        case UIGestureRecognizerStateRecognized:
            mInputProcess->pitchEnd();
            break;
        default:
            break;
    }
}

- (void) receiveModelLoadNotification:(NSNotification *) notification
{
    if ([[notification name] isEqualToString:@"ModelLoadNotification"]) {
        NSString *modelName = notification.object;
        
        [self loadModel:modelName];
        
        m_StepSlider.maximumValue = mModel->PieceCount();
        m_StepSlider.hidden = TRUE;

        m_buttonRestoreNext.enabled = false;
    }
}

- (void)togglePanRotateMode
{
    switch (m_dragMode) {
        case DRAG_IS_ROTATE:
            [self setDragModePan];
            break;
        case DRAG_IS_PAN:
            [self setDragModeRotate];
            break;
        default:
            break;
    }
}

- (void)resetZoom
{
    mLdrawRenderer->initViewMatrix(0);
    CGRect r = [self.view bounds];
    
    mInputProcess->setAspect(r.size.width, r.size.height);
    mLdrawRenderer->setProjectMatrix(r.size.width, r.size.height);

    mLdrawRenderer->updateTransform();
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    PartReportViewController *viewController = segue.destinationViewController;
    LegoModelPartsReport *report = [LegoModelPartsReport reportForModel:mModel withRenderer:mImageRender];
    
    viewController.Report = report;
}

- (void)showReport
{
    mModel->updateMovingPieces(1.0);
    if ( mImageRender == nil ) {
        CGSize size;
        size.width = 64;
        size.height = 64;
        mImageRender = [[ModelRenderToImage alloc] initWithSize:size shareGroup:self.context.sharegroup];
    }    
    [self performSegueWithIdentifier:@"partReportSegue" sender:self];
//    PartReportViewController *viewController = [self.storyboard instantiateViewControllerWithIdentifier:@"partReportViewControllerID"];
//    LegoModelPartsReport *report = [LegoModelPartsReport reportForModel:mModel withRenderer:mImageRender];
//    viewController.Report = report;
//    [self.navigationController pushViewController:viewController animated:YES];
}

- (void)initBars
{
    m_buttonRemoveNext = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Go_back.png"]
                          style:UIBarButtonItemStylePlain target:self action:@selector(removeNextPiece)];

    m_buttonRestoreNext = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Go_fwd.png"]
                          style:UIBarButtonItemStylePlain target:self action:@selector(restoreNextPiece)];
    
    NSArray *myButtonArray = [[NSArray alloc] initWithObjects:
//            [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Pan.png"]
//                                           style:UIBarButtonItemStylePlain
//                                          target:self action:@selector(setDragModePan)],
//            [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Rotate.png"]
//                                           style:UIBarButtonItemStylePlain
//                                          target:self action:@selector(setDragModeRotate)],
            [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Pan_Rotate.png"]
                                    style:UIBarButtonItemStylePlain target:self action:@selector(togglePanRotateMode)],

            [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
                                                        target:nil action:nil],
                              
            [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Report Card.png"]
                                           style:UIBarButtonItemStylePlain target:self action:@selector(showReport)],

          [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
                                            target:nil action:nil],
            //
            // reset zoom pozition
            //
            [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Original Size.png"]
                                           style:UIBarButtonItemStylePlain
                                          target:self action:@selector(resetZoom)],
            //
            // space to fwd/back
            //
            [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
                                              target:nil action:nil],

            m_buttonRemoveNext, m_buttonRestoreNext,
            nil];
    [self setToolbarItems:myButtonArray];
    
}

- (void)initLib
{
    mLdrawRenderer = new LdrawRender();
    mLdrawRenderer->loadDefaultShaders();
    mInputProcess = new InputProcess(mLdrawRenderer);

    NSString *libPathname = [[NSBundle mainBundle] pathForResource:@"library" ofType:@"bin"];
    mLdrawLib = new LdrawLib(libPathname.UTF8String);
    mModel = 0;
}

- (void)loadDefaultModel
{
    NSString *modelPath = [[NSBundle mainBundle] pathForResource:@"Simple Truck" ofType:@"ldr"];
#if TARGET_IPHONE_SIMULATOR
//    modelPath = @"/Users/lfroen/Dropbox/Lego/Fair Ground Spinner.ldr";
//    modelPath = @"/Users/lfroen/prog/mylego/lxf2ldr/test_1x1.lxf";
//    modelPath = @"/Users/lfroen/Dropbox/Lego/Fair Ground Spinner.lxf";
//    modelPath = @"/Users/lfroen/Dropbox/Lego/Big Yellow Dump Truck.ldr";
//    modelPath = @"/Users/lfroen/Dropbox/Lego/Red Dually Pickup N Camper.lxf";
    modelPath = @"/Users/lfroen/Dropbox/Lego/plane 1427920722m.lxf";
//    modelPath = @"/Users/lfroen/Dropbox/Lego/moc-3145.ldr";
//    modelPath = @"/Users/lfroen/Dropbox/Lego/Official_LEGO_Models/Star Wars/7150 - TIE Fighter & Y-Wing.mpd";
//    modelPath = @"/Users/lfroen/Dropbox/Lego/Official_LEGO_Models/Creator/10214 - Tower Bridge.mpd";
//        modelPath = @"/Users/lfroen/prog/mylego/submodel.mpd";
#endif
    [self loadModel:modelPath];
}

- (void)loadModel:(NSString *)fromFile
{
    mModel = 0;

    if ( [[fromFile.pathExtension uppercaseString] isEqualToString:@"LXF"] ) {
        if ( mXmlParser == nil ) {
            NSString *ldrawXmlPath = [[NSBundle mainBundle] pathForResource:@"ldraw" ofType:@"xml"];
            mXmlParser = [LdrawXmlParser ldrawXml:ldrawXmlPath];
        }
        lxfFile *lxf = [lxfFile lxfForPath:fromFile];

        std::map<std::string, std::vector<LEGO_PIECE_REF>> modelList;
        [lxf convertToLdrMesh:&modelList withXml:mXmlParser];
        mModel = LegoModel::createFromConvertedList(modelList, mLdrawLib);
    } else {
        NSData *fileData = [NSData dataWithContentsOfFile:fromFile];
        const char *p = (const char *)fileData.bytes;
        size_t len = fileData.length;
        char *buffer = new char [len + 1];
        strncpy(buffer, p, len);
        buffer[len] = 0;
        mModel = LegoModel::createFromBuffer(buffer, mLdrawLib);
        delete [] buffer;
    }
    
    mLdrawRenderer->setModel(mModel);
    
    mModel->loadGlBuffers();
    
    mLdrawRenderer->initDraw();
    
    m_StepSlider.maximumValue = mModel->PieceCount();
    
}

- (void)viewSizeChanged:(CGSize)size
{
    mInputProcess->setAspect(size.width, size.height);
    mLdrawRenderer->setProjectMatrix(size.width, size.height);
}

- (void)update
{
    mModel->updateMovingPieces(self.timeSinceLastUpdate);
    mLdrawRenderer->updateTransform();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    mLdrawRenderer->renderFrame();
}

-(IBAction)sliderValueChanged:(id)sender
{
    int intValue = m_StepSlider.maximumValue - m_StepSlider.value;
    NSLog(@"sliderValueChanged %d", intValue);
    mModel->setDisassemblyPosition(intValue);
}

-(void)setDragModePan
{
    UIImage *statusImage = [UIImage imageNamed:@"Pan.png"];
    if ( statusImage != nil ) {
        m_statusImage.image = statusImage;
    }
    mInputProcess->setModePan();
    m_dragMode = DRAG_IS_PAN;
}

-(void)setDragModeRotate
{
    UIImage *statusImage = [UIImage imageNamed:@"Rotate.png"];
    if ( statusImage != nil ) {
        m_statusImage.image = statusImage;
    }
    mInputProcess->setModeRotate();
    m_dragMode = DRAG_IS_ROTATE;
}


- (void)setSliderStep:(int)step
{
    m_StepSlider.value = m_StepSlider.maximumValue - step;
}


-(void)removeNextPiece
{
    m_StepSlider.hidden = FALSE;
    int step = mModel->moveNextPieceAway();
    m_StepSlider.value = m_StepSlider.maximumValue - step;
    m_buttonRestoreNext.enabled = true;
    if ( step == mModel->PieceCount() ) {
        m_buttonRemoveNext.enabled = false;
    }
}

-(void)restoreNextPiece
{
    int step = mModel->moveNextPieceBack();
    m_StepSlider.value = m_StepSlider.maximumValue - step;
    m_buttonRemoveNext.enabled = true;
    if ( step == 0 ) {
        m_StepSlider.hidden = TRUE;
        m_buttonRestoreNext.enabled = false;
    }
}

- (void)viewWillTransitionToSize:(CGSize)size
       withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    [self viewSizeChanged:size];
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}

@end


