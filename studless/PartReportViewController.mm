//
//  PartReportViewController.m
//  simpleGL
//
//  Created by Leonid on 30/09/2015.
//  Copyright © 2015 lFroen. All rights reserved.
//

#import "PartReportViewController.h"
#import "ModelRenderToImage.h"

inline double radians (double degrees) { return degrees * M_PI/180; }

void drawLinearGradient(CGContextRef context, CGRect rect, CGColorRef startColor, CGColorRef endColor)
{
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGFloat locations[] = { 0.0, 1.0 };
    
    NSArray *colors = @[(__bridge id) startColor, (__bridge id) endColor];
    
    CGGradientRef gradient = CGGradientCreateWithColors(colorSpace, (__bridge CFArrayRef) colors, locations);
    
    CGPoint startPoint = CGPointMake(CGRectGetMidX(rect), CGRectGetMinY(rect));
    CGPoint endPoint = CGPointMake(CGRectGetMidX(rect), CGRectGetMaxY(rect));
    
    CGContextSaveGState(context);
    CGContextAddRect(context, rect);
    CGContextClip(context);
    CGContextDrawLinearGradient(context, gradient, startPoint, endPoint, 0);
    CGContextRestoreGState(context);
    
    CGGradientRelease(gradient);
    CGColorSpaceRelease(colorSpace);
}

void draw1PxStroke(CGContextRef context, CGPoint startPoint, CGPoint endPoint, CGColorRef color)
{
    
    CGContextSaveGState(context);
    CGContextSetLineCap(context, kCGLineCapSquare);
    CGContextSetStrokeColorWithColor(context, color);
    CGContextSetLineWidth(context, 1.0);
    CGContextMoveToPoint(context, startPoint.x + 0.5, startPoint.y + 0.5);
    CGContextAddLineToPoint(context, endPoint.x + 0.5, endPoint.y + 0.5);
    CGContextStrokePath(context);
    CGContextRestoreGState(context);
    
}

CGRect rectFor1PxStroke(CGRect rect)
{
    return CGRectMake(rect.origin.x + 0.5, rect.origin.y + 0.5, rect.size.width - 1, rect.size.height - 1);
}

void drawGlossAndGradient(CGContextRef context, CGRect rect, CGColorRef startColor, CGColorRef endColor)
{
    drawLinearGradient(context, rect, startColor, endColor);
    
    UIColor * glossColor1 = [UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:0.35];
    UIColor * glossColor2 = [UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:0.1];
    
    CGRect topHalf = CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height/2);
    
    drawLinearGradient(context, topHalf, glossColor1.CGColor, glossColor2.CGColor);
}

CGMutablePathRef createArcPathFromBottomOfRect(CGRect rect, CGFloat arcHeight)
{
    CGRect arcRect = CGRectMake(rect.origin.x, rect.origin.y + rect.size.height - arcHeight, rect.size.width, arcHeight);
    
    CGFloat arcRadius = (arcRect.size.height/2) + (pow(arcRect.size.width, 2) / (8*arcRect.size.height));
    CGPoint arcCenter = CGPointMake(arcRect.origin.x + arcRect.size.width/2, arcRect.origin.y + arcRadius);
    
    CGFloat angle = acos(arcRect.size.width / (2*arcRadius));
    CGFloat startAngle = radians(180) + angle;
    CGFloat endAngle = radians(360) - angle;
    
    CGMutablePathRef path = CGPathCreateMutable();
    CGPathAddArc(path, NULL, arcCenter.x, arcCenter.y, arcRadius, startAngle, endAngle, 0);
    CGPathAddLineToPoint(path, NULL, CGRectGetMaxX(rect), CGRectGetMinY(rect));
    CGPathAddLineToPoint(path, NULL, CGRectGetMinX(rect), CGRectGetMinY(rect));
    CGPathAddLineToPoint(path, NULL, CGRectGetMinX(rect), CGRectGetMaxY(rect));
    
    return path;
}

@interface CustomCellBackground : UIView

@property (nonatomic, assign) BOOL selected;

@end

@implementation CustomCellBackground

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

-(void) drawRect: (CGRect) rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    UIColor * whiteColor = [UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:1.0];
//    UIColor * lightGrayColor = [UIColor colorWithRed:230.0/255.0 green:230.0/255.0 blue:230.0/255.0 alpha:1.0];
    UIColor * lightGrayColor = [UIColor colorWithRed:30.0/255.0 green:30.0/255.0 blue:30.0/255.0 alpha:1.0];
    UIColor * separatorColor = [UIColor colorWithRed:208.0/255.0 green:208.0/255.0 blue:208.0/255.0 alpha:1.0];
    
    CGRect paperRect = self.bounds;
    
    if (self.selected) {
        drawLinearGradient(context, paperRect, lightGrayColor.CGColor, separatorColor.CGColor);
    } else {
        drawLinearGradient(context, paperRect, whiteColor.CGColor, lightGrayColor.CGColor);
    }
    
    drawLinearGradient(context, paperRect, whiteColor.CGColor, lightGrayColor.CGColor);
    
    CGRect strokeRect = paperRect;
    strokeRect.size.height -= 1;
    strokeRect = rectFor1PxStroke(strokeRect);
    
    CGContextSetStrokeColorWithColor(context, whiteColor.CGColor);
    
    CGContextSetLineWidth(context, 1.0);
    CGContextStrokeRect(context, strokeRect);
    
    CGContextSetStrokeColorWithColor(context, whiteColor.CGColor);
    CGContextSetLineWidth(context, 1.0);
    
    CGPoint pointA = CGPointMake(paperRect.origin.x, paperRect.origin.y + paperRect.size.height - 1);
    CGPoint pointB = CGPointMake(paperRect.origin.x, paperRect.origin.y);
    CGPoint pointC = CGPointMake(paperRect.origin.x + paperRect.size.width - 1, paperRect.origin.y);
    CGPoint pointD = CGPointMake(paperRect.origin.x + paperRect.size.width - 1, paperRect.origin.y + paperRect.size.height - 1);
    
    draw1PxStroke(context, pointA, pointB, whiteColor.CGColor);
    draw1PxStroke(context, pointB, pointC, whiteColor.CGColor);
    draw1PxStroke(context, pointC, pointD, whiteColor.CGColor);
}

@end

static NSString *SectionHeaderViewIdentifier = @"SectionHeaderViewIdentifier";

@interface PartReportViewController ()

@property (nonatomic) NSMutableArray *sectionInfoArray;


@end

@implementation PartReportViewController

@synthesize Report = mReport;

static NSString *CellIdentifier = @"PartReportTableCell";
static NSString *HeaderIdentifier = @"PartReportTableHeaderCell";
static NSString *ColorCellIdentifier = @"PartReportTableColorCell";

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.tableView.rowHeight = 28;
    self.tableView.sectionHeaderHeight = 64;
    
    self.sectionInfoArray = [[NSMutableArray alloc] init];

    for (LegoModelPartsReportIem *i in self.Report.Items) {
        [self.sectionInfoArray addObject:i];
    }
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(receiveModelLoadNotification:)
                                                 name:@"ModelLoadNotification"
                                               object:nil];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [[self navigationController] setNavigationBarHidden:NO animated:YES];
    [[self navigationController] setToolbarHidden:YES];
}

- (void) receiveModelLoadNotification:(NSNotification *) notification
{
    //
    // when new model is loaded, close report view as irrelevant
    //
    if ([[notification name] isEqualToString:@"ModelLoadNotification"]) {
        [self.navigationController popViewControllerAnimated:YES];
    }
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    PartReportTableViewColorCell *cell = nil;

    cell = [tableView dequeueReusableCellWithIdentifier:ColorCellIdentifier forIndexPath:indexPath];
//    if (![cell.backgroundView isKindOfClass:[CustomCellBackground class]]) {
//        CustomCellBackground * backgroundCell = [[CustomCellBackground alloc] init];
//        cell.backgroundView = backgroundCell;
//    }
//    
//    if (![cell.selectedBackgroundView isKindOfClass:[CustomCellBackground class]]) {
//        CustomCellBackground * selectedBackgroundCell = [[CustomCellBackground alloc] init];
//        selectedBackgroundCell.selected = YES;
//        cell.selectedBackgroundView = selectedBackgroundCell;
//    }
//    
    NSInteger sectionIndex = [indexPath indexAtPosition:0];
    NSInteger index = [indexPath indexAtPosition:1];
    LegoModelPartsReportIem *it = [self.Report.Items objectAtIndex:sectionIndex];
//    cell.mainLabel.text = it.Description;
//    cell.countLabel.text = [[NSString alloc] initWithFormat:@"x%d", it.TotalUsedCount];
//    cell.partImage.image = it.Image;
    cell.countLabel.text = [[NSString alloc] initWithFormat:@" x%d : %@",
                            [it colorCount:(int)index], [it colorNameAtIndex:(int)index]];
    UIColor *color = [it colorAtIndex:(int)index];
    CGFloat white;
    [color getWhite:&white alpha:0];
    if ( white < 0.1 ) {
        cell.countLabel.textColor = [UIColor whiteColor];
    }
    cell.countLabel.backgroundColor = color;
    
    return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    NSInteger TotalCount = self.Report.Items.count;
    return TotalCount;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    LegoModelPartsReportIem *reportItem = (LegoModelPartsReportIem *)self.Report.Items[section];
    return reportItem.Expanded ? reportItem.ColorCount : 0;
}

//- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
//{
//    NSInteger itemCount = [mReport.Items count];
//    return itemCount;
//}
- (IBAction)toggleOpen:(id)sender {
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    PartReportTableViewHeaderCell *cell = nil;
    
    cell = [tableView dequeueReusableCellWithIdentifier:HeaderIdentifier];
    LegoModelPartsReportIem *it = [self.Report.Items objectAtIndex:section];
    if (![cell.backgroundView isKindOfClass:[CustomCellBackground class]]) {
        CustomCellBackground * backgroundCell = [[CustomCellBackground alloc] init];
        cell.backgroundView = backgroundCell;
    }

    if (![cell.selectedBackgroundView isKindOfClass:[CustomCellBackground class]]) {
        CustomCellBackground * selectedBackgroundCell = [[CustomCellBackground alloc] init];
        selectedBackgroundCell.selected = YES;
        cell.selectedBackgroundView = selectedBackgroundCell;
    }
    cell.mainLabel.text = it.Description;
    cell.countLabel.text = [[NSString alloc] initWithFormat:@" x%d", it.TotalUsedCount];
    cell.partImage.image = it.Image;
    cell.section = section;
    cell.delegate = self;
    
    if ( it.ColorCount == 1 ) {
        cell.expandButton.hidden = true;
    } else {
        [cell.expandButton addTarget:cell action:@selector(toggleOpen:) forControlEvents:UIControlEventPrimaryActionTriggered];
    }
//        return cell;
    //
    // following is workaround for view being confused when cellview is returned
    // as section header. Code taken from
    // http://stackoverflow.com/questions/12772197/what-is-the-meaning-of-the-no-index-path-for-table-cell-being-reused-message-i
    //
    // unfortunately, such workaround doesn't honor sectionHeaderHeight setting, and view width - so force it manually
    //
    
    CGRect r = cell.frame;
    r.origin.y = 0;
    r.size.height = self.tableView.sectionHeaderHeight;
    r.size.width = self.view.frame.size.width;
    cell.frame = r;
    UIView *view = [[UIView alloc] initWithFrame:r];
    [view addSubview:cell];
    return view;
}

- (void)sectionHeaderView:(UIView *)sectionHeaderView sectionOpened:(NSInteger)sectionOpened
{
//    PartReportTableViewHeaderCell *cell = (PartReportTableViewHeaderCell *)sectionHeaderView;
//    cell.expandButton.imageView.image = nil;
//    [cell.expandButton setImage:[UIImage imageNamed:@"Close-20.png"] forState:UIControlStateNormal];
    /*
     Create an array containing the index paths of the rows to insert: These correspond
     to the rows for each quotation in the current section.
     */
    LegoModelPartsReportIem *reportItem = (LegoModelPartsReportIem *)self.Report.Items[sectionOpened];
    NSInteger countOfRowsToInsert = reportItem.ColorCount;

    reportItem.Expanded = TRUE;

    NSMutableArray *indexPathsToInsert = [[NSMutableArray alloc] init];
    for (NSInteger i = 0; i < countOfRowsToInsert; i++) {
        [indexPathsToInsert addObject:[NSIndexPath indexPathForRow:i inSection:sectionOpened]];
    }
    
    
    // style the animation so that there's a smooth flow in either direction
//    UITableViewRowAnimation insertAnimation;
//    UITableViewRowAnimation deleteAnimation;
//    if (previousOpenSectionIndex == NSNotFound || sectionOpened < previousOpenSectionIndex) {
//        insertAnimation = UITableViewRowAnimationTop;
//        deleteAnimation = UITableViewRowAnimationBottom;
//    }
//    else {
//        insertAnimation = UITableViewRowAnimationBottom;
//        deleteAnimation = UITableViewRowAnimationTop;
//    }
//    
    // apply the updates
    [self.tableView beginUpdates];
    [self.tableView insertRowsAtIndexPaths:indexPathsToInsert withRowAnimation:UITableViewRowAnimationTop];
//    [self.tableView deleteRowsAtIndexPaths:indexPathsToDelete withRowAnimation:deleteAnimation];
    [self.tableView endUpdates];
    
//    self.openSectionIndex = sectionOpened;
}

- (void)sectionHeaderView:(UIView *)sectionHeaderView sectionClosed:(NSInteger)sectionClosed
{
    LegoModelPartsReportIem *reportItem = (LegoModelPartsReportIem *)self.Report.Items[sectionClosed];
//    NSInteger countOfRowsToInsert = reportItem.ColorCount;
    /*
     Create an array of the index paths of the rows in the section that was closed, then delete those rows from the table view.
     */
    
    NSInteger countOfRowsToDelete = [self.tableView numberOfRowsInSection:sectionClosed];
    reportItem.Expanded = FALSE;
    
    if (countOfRowsToDelete > 0) {
        NSMutableArray *indexPathsToDelete = [[NSMutableArray alloc] init];
        for (NSInteger i = 0; i < countOfRowsToDelete; i++) {
            [indexPathsToDelete addObject:[NSIndexPath indexPathForRow:i inSection:sectionClosed]];
        }
        [self.tableView deleteRowsAtIndexPaths:indexPathsToDelete withRowAnimation:UITableViewRowAnimationTop];
    }
}

@end


@implementation PartReportTableViewCell


@end

@implementation PartReportTableViewColorCell


@end

@implementation PartReportTableViewHeaderCell

- (IBAction)toggleOpen:(id)sender
{
    self.expandButton.selected = !self.expandButton.selected;
    if (self.expandButton.selected) {
        if ([self.delegate respondsToSelector:@selector(sectionHeaderView:sectionOpened:)]) {
            [self.delegate sectionHeaderView:self sectionOpened:self.section];
            [self.expandButton setImage:[UIImage imageNamed:@"Triangular Arrow Down-30.png"] forState:UIControlStateNormal] ;
        }
    }
    else {
        if ([self.delegate respondsToSelector:@selector(sectionHeaderView:sectionClosed:)]) {
            [self.delegate sectionHeaderView:self sectionClosed:self.section];
            [self.expandButton setImage:[UIImage imageNamed:@"Triangular Arrow Right-30.png"] forState:UIControlStateNormal] ;
        }
    }
}

@end


@implementation PartReportDetailViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    UIStoryboard *myStoryboard = [UIStoryboard storyboardWithName:@"Main"
                                                           bundle:[NSBundle mainBundle]];
    mPartView = [myStoryboard instantiateViewControllerWithIdentifier:@"modelReportDetailedGlViewId"];
    [self.view addSubview:mPartView.view];
    [self addChildViewController:mPartView];
    [mPartView didMoveToParentViewController:self];
    CGRect frameBounds = self.view.bounds;
    mPartView.view.frame = frameBounds;
}

@end

@interface PartReportDetailGLKViewController () {
    
}
@property (strong, nonatomic) EAGLContext *context;
@end

@implementation PartReportDetailGLKViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClearColor(1.0, 0.0, 0.0, 1.1);
}

@end
