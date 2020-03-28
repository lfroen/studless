//
//  LdrawXmlParser.h
//  simpleGL
//
//  Created by Leonid on 20/10/2015.
//  Copyright © 2015 lFroen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <TargetConditionals.h>
#include "vec_math.h"

#if TARGET_OS_IOS
#include "legomesh.h"
#endif

@interface LDD2LDRAW : NSObject<NSCoding> {
}

@property NSString *ldd;
@property NSString *ldr;

@property Vector3 offset;
@property Vector3 rotationAxis;
@property float rotationAngle;

@property (readonly,getter=getRotation) Matrix33 Rotation;

- (Matrix33)getRotation;

@end

@interface LdrawXmlParser : NSObject<NSXMLParserDelegate> {
    NSMutableDictionary *mTransformLU;
    NSMutableDictionary *mMaterialLU;
    
    NSMutableDictionary *mLdr2LddLU;
    
    NSNumberFormatter *mFormatter;
}

+ (LdrawXmlParser *)ldrawXml:(NSString *)path;
+ (LdrawXmlParser *)ldrawXmlFromFile:(NSString *)path;

- (LdrawXmlParser *)init;
- (bool)parseFile:(NSString *)path;

- (LDD2LDRAW *)transformFor:(NSString *)lddID;
- (long)ldrColorFor:(NSString *)lddColor;

- (void)saveTransformDictionaryAs:(NSString *)path;


@end

@interface lddPart : NSObject {
    
}
@property NSString *legoID;

@property Matrix33 rotation;
@property Vector3 position;
@property NSString *color;

@end

@interface lxfFile : NSObject<NSXMLParserDelegate> {
    NSMutableArray *mParts;
}

+ (lxfFile *)lxfForPath:(NSString *)path;
- (bool)parseFile:(NSString *)path;

- (void)exportTo:(NSString *)path withXml:(LdrawXmlParser *)withXml;

#if TARGET_OS_IOS
- (void)convertToLdrMesh:(std::map<std::string,std::vector<LEGO_PIECE_REF>> *)ldrModel withXml:(LdrawXmlParser *)withXml;
#endif

@end

