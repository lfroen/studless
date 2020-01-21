//
//  NSObject_LegoModelPartsReport_mm.h
//  simpleGL
//
//  Created by Leonid on 05/10/2015.
//  Copyright © 2015 lFroen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "legomesh.h"

//
// Idea is to convert CPP representation to ObjC to allow it to be passed
// into ARC-enabled code
//

@class ModelRenderToImage;

//
// each report item is referenced (1 or more times) part
//
@interface LegoModelPartsReportIem : NSObject {
    UIImage *mImage;
    NSString *mDescription;

    int mTotalUsedCount;
    NSMutableDictionary *mColorDictionary;
    
    ModelRenderToImage *mImageRenderer;
    LegoModel *mModel;
    LegoPiece *mPiece;
    
}
@property NSString *Description;
@property int TotalUsedCount;
@property (readonly, getter=getImage) UIImage *Image;

@property (readonly, getter=colorCount) int ColorCount;

@property BOOL Expanded;

- (id) initWithLegoPiece:(LegoPiece *)piece withModel:(LegoModel *)withModel withRenderer:(ModelRenderToImage *)withRenderer;

- (void) addColorRef:(int)colorCode withCount:(int)withCount;

- (int) colorCount;
- (int) colorCount:(int)colorIndex;

- (UIColor *)colorAtIndex:(int)colorIndex;
- (NSString *)colorNameAtIndex:(int)colorIndex;

- (UIImage *)getImage;

@end

@interface LegoModelPartsReport : NSObject {
    NSMutableArray *mItems;
}

@property NSMutableArray *Items;

+ (id) reportForModel:(LegoModel *)model withRenderer:(ModelRenderToImage *)withRenderer;

@end

