//
//  GameViewController.h
//  legoview
//
//  Created by Leonid on 8/3/15.
//  Copyright (c) 2015 lFroen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include "legomesh.h"

#import "LdrawXmlParser.h"

typedef enum : NSUInteger {
    DRAG_IS_PAN,
    DRAG_IS_ROTATE,
} DRAG_MODE;

@interface CustomGLKViewController : GLKViewController {
    IBOutlet GLKView* m_oglView;
    
    IBOutlet UIImageView *m_statusImage;
    IBOutlet UISlider *m_StepSlider;

    UIBarButtonItem *m_buttonPanRotate;
    UIBarButtonItem *m_buttonRemoveNext;
    UIBarButtonItem *m_buttonRestoreNext;
    
    LdrawLib *mLdrawLib;
    LegoModel *mModel;
    LdrawRender *mLdrawRenderer;

    LdrawXmlParser *mXmlParser;
    
    DRAG_MODE m_dragMode;
    InputProcess *mInputProcess;
    
#if TARGET_IPHONE_SIMULATOR
#endif
}
@property (strong, nonatomic) EAGLContext *context;

@property (nonatomic, readonly) LdrawRender *Renderer;
@property (nonatomic, readonly) LegoModel *Model;

- (void)initLib;
- (void)loadModel:(NSString *)fromFile;

- (void)viewSizeChanged:(CGSize)size;

- (void)update;
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect;

- (void)setSliderStep:(int)step;
- (IBAction)sliderValueChanged:(id)sender;

- (void)removeNextPiece;
- (void)restoreNextPiece;

- (IBAction)handlePanGestureWithGestureRecognizer:(UIPanGestureRecognizer *)gestureRecognizer;
- (IBAction)handleTapGestureWithGestureRecognizer:(UITapGestureRecognizer *)gestureRecognizer;
- (IBAction)handlePinchGestureWithGestureRecognizer:(UIPinchGestureRecognizer *)gestureRecognizer;

- (void)receiveModelLoadNotification:(NSNotification *) notification;

@end

