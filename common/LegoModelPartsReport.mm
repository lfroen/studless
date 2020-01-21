//
//  LegoModelPartsReport.m
//  simpleGL
//
//  Created by Leonid on 05/10/2015.
//  Copyright © 2015 lFroen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "LegoModelPartsReport.h"

#import "ModelRenderToImage.h"

#include "legomesh.h"

#include "ldrawcolor.h"

@implementation LegoModelPartsReportIem

@synthesize Description = mDescription;
@synthesize TotalUsedCount = mTotalUsedCount;
@synthesize Image = mImage;

//@synthesize ColorCount;

- (id)initWithLegoPiece:(LegoPiece *)piece withModel:(LegoModel *)withModel withRenderer:(ModelRenderToImage *)withRenderer
{
//    int colorCode = 27;
//    uint32_t rgbColor = colorForCode(colorCode);
//    uint32_t complementColor = complementColorForCode(colorCode);
//    Vector4 colorVec(LC_RGBA_RED(rgbColor), LC_RGBA_GREEN(rgbColor),
//                     LC_RGBA_BLUE(rgbColor), LC_RGBA_ALPHA(rgbColor));
//    Vector4 complementColorVec(LC_RGBA_RED(complementColor), LC_RGBA_GREEN(complementColor),
//                               LC_RGBA_BLUE(complementColor), LC_RGBA_ALPHA(complementColor));
//    
//    LEGO_PIECE_REF pieceRef;
//    pieceRef.hidden = false;
//    pieceRef.colorVec = colorVec;
//    pieceRef.complementColorVec = complementColorVec;
//    pieceRef.transform = Matrix44Identity();
//    
//    UIImage *image = [withRenderer renderModel:withModel withPiece:piece withRef:&pieceRef];
//    mImage = image;
    self = [super init];
    mImage = nil;
    
    mModel = withModel;
    mPiece = piece;
    mImageRenderer = withRenderer;
    
    self.Description = [NSString stringWithUTF8String:piece->Description()];

    self.TotalUsedCount = 0;
    mColorDictionary = [[NSMutableDictionary alloc] init];
    return self;
}

- (void) addColorRef:(int)colorCode withCount:(int)withCount
{
    mTotalUsedCount += withCount;
    
    NSNumber *colorCodeKey = [NSNumber numberWithInt:colorCode];
    NSNumber *currCount = [mColorDictionary objectForKey:colorCodeKey];
    if ( currCount == nil ) {
        currCount = [NSNumber numberWithInt:withCount];
    } else {
        currCount = [NSNumber numberWithInt:withCount + currCount.intValue];
    }
    [mColorDictionary setObject:currCount forKeyedSubscript:colorCodeKey];
}

- (UIImage *)getImage
{
    if ( mImage == nil ) {
        int colorCode = 7; // Gray by default
        if ( mColorDictionary.allKeys.count == 1 ) {
            NSNumber *colorKey = [mColorDictionary.allKeys objectAtIndex:0];
            colorCode = (int)colorKey.integerValue;
        }
        uint32_t rgbColor = colorForCode(colorCode);
        uint32_t complementColor = complementColorForCode(colorCode);
        Vector4 colorVec(LC_RGBA_RED(rgbColor), LC_RGBA_GREEN(rgbColor),
                         LC_RGBA_BLUE(rgbColor), LC_RGBA_ALPHA(rgbColor));
        Vector4 complementColorVec(LC_RGBA_RED(complementColor), LC_RGBA_GREEN(complementColor),
                                   LC_RGBA_BLUE(complementColor), LC_RGBA_ALPHA(complementColor));
        LEGO_PIECE_REF pieceRef;
        pieceRef.hidden = false;
        pieceRef.colorVec = colorVec;
        pieceRef.complementColorVec = complementColorVec;
        pieceRef.transform = Matrix44Identity();
        
        UIImage *image = [mImageRenderer renderModel:mModel withPiece:mPiece withRef:&pieceRef];
        mImage = image;
    }
    return mImage;
}

- (int) colorCount
{
    return (int)mColorDictionary.allKeys.count;
}

- (int) colorCount:(int)colorIndex
{
    NSNumber *colorKey = [mColorDictionary.allKeys objectAtIndex:colorIndex];
    NSNumber *colorCount = [mColorDictionary objectForKey:colorKey];
    return (colorCount == nil) ? 0 : colorCount.intValue;
}

#define L_RGBA_RED(rgba)   ((((rgba) >>  0) & 0xff) / 255.0)
#define L_RGBA_GREEN(rgba) ((((rgba) >>  8) & 0xff) / 255.0)
#define L_RGBA_BLUE(rgba)  ((((rgba) >> 16) & 0xff) / 255.0)
#define L_RGBA_ALPHA(rgba) ((((rgba) >> 24) & 0xff) / 255.0)

- (UIColor *)colorAtIndex:(int)colorIndex
{
//    printf("%s: object=%s index=%d keys=%lul\n", __func__, self.Description.UTF8String, colorIndex, (unsigned long)mColorDictionary.allKeys.count);
    assert(mColorDictionary.allKeys.count > colorIndex);
    NSNumber *colorKey = [mColorDictionary.allKeys objectAtIndex:colorIndex];
    assert(colorKey != nil);
    NSInteger colorCode = colorKey.integerValue;
    NSInteger rgbColor = g_ldrawColors[colorCode].color;
    UIColor *color = [UIColor colorWithRed:L_RGBA_RED(rgbColor) green:L_RGBA_GREEN(rgbColor)
                                      blue:L_RGBA_BLUE(rgbColor) alpha:L_RGBA_ALPHA(rgbColor)];
    return color;
}

- (NSString *)colorNameAtIndex:(int)colorIndex
{
    NSNumber *colorKey = [mColorDictionary.allKeys objectAtIndex:colorIndex];
    NSInteger colorCode = colorKey.integerValue;
    NSString *colorName = [NSString stringWithUTF8String:g_ldrawColors[colorCode].name];
    return colorName;
}


@end

@implementation LegoModelPartsReport

@synthesize Items = mItems;

+ (id) reportForModel:(LegoModel *)model withRenderer:(ModelRenderToImage *)withRenderer
{
    //
    // query cpp object for data
    //
    LEGO_MODEL_REPORT cppReport;
    model->createReport(cppReport);
    
    //
    // now load it to ObjC domain
    //
    LegoModelPartsReport *report = [LegoModelPartsReport alloc];
    size_t reportSize = cppReport.size();
    report->mItems = [NSMutableArray arrayWithCapacity:reportSize];
    NSInteger index = 0;
    for (auto i = cppReport.begin(); i != cppReport.end(); i++) {
        LegoModelPartsReportIem *item = [[LegoModelPartsReportIem alloc]
                                         initWithLegoPiece:i->first withModel:model
                                         withRenderer:withRenderer];
        for (auto j = i->second.begin(); j != i->second.end(); j++) {
            [item addColorRef:j->first withCount:j->second];
        }
        [report->mItems setObject:item atIndexedSubscript:index++];
    }
    return report;
}

@end
