//
//  PartReportViewController.h
//  simpleGL
//
//  Created by Leonid on 30/09/2015.
//  Copyright © 2015 lFroen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#import "LegoModelPartsReport.h"

#import "ModelRenderToImage.h"

//
// Open-close tableview section
//
@protocol SectionHeaderViewDelegate;


@interface APLSectionHeaderView : UITableViewHeaderFooterView


@property (nonatomic, weak) IBOutlet UILabel *titleLabel;
@property (nonatomic, weak) IBOutlet UIButton *disclosureButton;
@property (nonatomic, weak) IBOutlet id <SectionHeaderViewDelegate> delegate;

@property (nonatomic) NSInteger section;

- (void)toggleOpenWithUserAction:(BOOL)userAction;

@end


/*
 Protocol to be adopted by the section header's delegate; the section header tells
 its delegate when the section should be opened and closed.
 */
@protocol SectionHeaderViewDelegate <NSObject>


@optional

- (void)sectionHeaderView:(UIView *)sectionHeaderView sectionOpened:(NSInteger)section;
- (void)sectionHeaderView:(UIView *)sectionHeaderView sectionClosed:(NSInteger)section;

@end


@interface PartReportViewController : UITableViewController <UITableViewDelegate, SectionHeaderViewDelegate> {
    LegoModelPartsReport *mReport;
}

@property LegoModelPartsReport *Report;


@end

@interface PartReportTableViewCell : UITableViewCell {
}

@property (weak, nonatomic) IBOutlet UILabel *mainLabel;
@property (weak, nonatomic) IBOutlet UILabel *countLabel;
@property (weak, nonatomic) IBOutlet UIImageView *partImage;


@end

@interface PartReportTableViewHeaderCell : UITableViewCell {
}

@property (weak, nonatomic) IBOutlet UIButton *expandButton;

@property (weak, nonatomic) IBOutlet UILabel *mainLabel;
@property (weak, nonatomic) IBOutlet UILabel *countLabel;
@property (weak, nonatomic) IBOutlet UIImageView *partImage;

@property (nonatomic) NSInteger section;

@property (nonatomic, weak) IBOutlet id <SectionHeaderViewDelegate> delegate;

@end

@interface PartReportTableViewColorCell : UITableViewCell {
}

@property (weak, nonatomic) IBOutlet UILabel *countLabel;


@end

@interface PartReportDetailGLKViewController : GLKViewController {
    
}
@end

@interface PartReportDetailViewController : UIViewController {
    GLKViewController *mPartView;
}

@end

