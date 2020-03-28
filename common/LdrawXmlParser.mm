//
//  LdrawXmlParser.m
//  simpleGL
//
//  Created by Leonid on 20/10/2015.
//  Copyright © 2015 lFroen. All rights reserved.
//

#import "LdrawXmlParser.h"

#if TARGET_OS_IOS
#include "legomesh.h"
#endif

#include "unzip.h"

@implementation LDD2LDRAW

- (Matrix33)getRotation
{
    float cosTheta = cosf(_rotationAngle);
    float oneMinCosTherta = 1.0 - cosTheta;
    float sinTheta = sinf(_rotationAngle);
    float ux = _rotationAxis.x;
    float uy = _rotationAxis.y;
    float uz = _rotationAxis.z;
        
    Vector3 m0(cosTheta + ux*ux*oneMinCosTherta,    ux*uy*oneMinCosTherta - uz*sinTheta, uz*ux*oneMinCosTherta + uy*sinTheta);
    Vector3 m1(ux*uy*oneMinCosTherta + uz*sinTheta, cosTheta + uy*uy*oneMinCosTherta, uz*uy*oneMinCosTherta - ux*sinTheta);
    Vector3 m2(ux*uz*oneMinCosTherta - uy*sinTheta, uy*uz*oneMinCosTherta + ux*sinTheta, cosTheta + uz*uz*oneMinCosTherta);
        
    Matrix33 m = Matrix33(m0, m1, m2);

    return m;
}

- (void)encodeWithCoder:(NSCoder *)coder
{
    [coder encodeObject:self.ldd forKey:@"ldd"];
    [coder encodeObject:self.ldr forKey:@"ldr"];
    
    NSNumber *ax = [NSNumber numberWithFloat:_rotationAxis.x];
    [coder encodeObject:ax forKey:@"ax"];
    NSNumber *ay = [NSNumber numberWithFloat:_rotationAxis.y];
    [coder encodeObject:ay forKey:@"ay"];
    NSNumber *az = [NSNumber numberWithFloat:_rotationAxis.z];
    [coder encodeObject:az forKey:@"az"];

    NSNumber *offx = [NSNumber numberWithFloat:_offset.x];
    [coder encodeObject:offx forKey:@"offx"];
    NSNumber *offy = [NSNumber numberWithFloat:_offset.y];
    [coder encodeObject:offy forKey:@"offy"];
    NSNumber *offz = [NSNumber numberWithFloat:_offset.z];
    [coder encodeObject:offz forKey:@"offz"];
}

- (id)initWithCoder:(NSCoder *)coder {
    
    self = [super init];
    
    if (self) {
        self.ldd = [coder decodeObjectForKey:@"ldd"];
        self.ldr = [coder decodeObjectForKey:@"ldr"];
        
        NSNumber *ax = [coder decodeObjectForKey:@"ax"];
        NSNumber *ay = [coder decodeObjectForKey:@"ay"];
        NSNumber *az = [coder decodeObjectForKey:@"az"];
        _rotationAxis = Vector3(ax.floatValue, ay.floatValue, az.floatValue);
        
        NSNumber *offx = [coder decodeObjectForKey:@"offx"];
        NSNumber *offy = [coder decodeObjectForKey:@"offy"];
        NSNumber *offz = [coder decodeObjectForKey:@"offz"];
        _rotationAxis = Vector3(offx.floatValue, offy.floatValue, offz.floatValue);
        
    }
    return self;
}

@end


@implementation LdrawXmlParser

- (LdrawXmlParser *)init
{
    self = [super init];
    
    mFormatter = [[NSNumberFormatter alloc] init];
    mFormatter.numberStyle = NSNumberFormatterDecimalStyle;

    mMaterialLU = [NSMutableDictionary dictionary];
    mTransformLU = [NSMutableDictionary dictionary];
    
    mLdr2LddLU = [NSMutableDictionary dictionary];
    return self;
}


+ ldrawXml:(NSString *)path
{
    LdrawXmlParser *ldr = [[LdrawXmlParser alloc] init];
    
    [ldr parseFile:path];
    
    return ldr;
}

- (bool)parseFile:(NSString *)path
{
    NSError *error = nil;
    
    // Load the file and check the result
    NSData *data = [NSData dataWithContentsOfFile:path
                                          options:NSDataReadingUncached error:&error];
    
    if(error) {
        NSLog(@"Error %@", error);
        return false;
    }
    
    NSXMLParser *parser = [[NSXMLParser alloc] initWithData:data];
    [parser setDelegate:self];
    [parser parse];
    error = [parser parserError];
    
    if(error) {
        NSLog(@"Error %@", error);
        return false;
    }
    return true;
}

//- (void) parserDidStartDocument:(NSXMLParser *)parser
//{
//    NSLog(@"parserDidStartDocument");
//}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName
    namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
    attributes:(NSDictionary *)attributeDict
{
    if ( [elementName isEqualToString:@"Material"] ) {
        //
        // <Material ldraw="33" lego="43" />
        //
        NSString *ldraw = [attributeDict valueForKey:@"ldraw"];
        NSString *lego = [attributeDict valueForKey:@"lego"];
        [mMaterialLU setValue:ldraw forKey:lego];
//        NSLog(@"didStartElement --> %@", elementName);
    } else if ([elementName isEqualToString:@"Transformation"] ) {
        //
        // <Transformation ldraw="2340.dat" tx="0" ty="-2.88" tz=".4" ax="0" ay="1" az="0" angle="4.712389" />
        //
        NSString *ldraw = [attributeDict valueForKey:@"ldraw"];
        ldraw = [ldraw stringByDeletingPathExtension];
        
        NSString *lego = [mLdr2LddLU valueForKey:ldraw];
        LDD2LDRAW *ldd2ldraw = nil;
        //
        // if we have lego ID - look for transform. Otherwise, there's no point
        // in looking - it's not there anyway
        //
        if ( lego == nil ) {
            lego = ldraw;
        } else {
            ldd2ldraw = [mTransformLU valueForKey:ldraw];
        }
        if ( ldd2ldraw == nil ) {
            ldd2ldraw = [[LDD2LDRAW alloc] init];
            ldd2ldraw.ldr = ldraw;
            ldd2ldraw.ldd = lego;
        }

        NSNumber *tx = [mFormatter numberFromString:[attributeDict valueForKey:@"tx"]];
        NSNumber *ty = [mFormatter numberFromString:[attributeDict valueForKey:@"ty"]];
        NSNumber *tz = [mFormatter numberFromString:[attributeDict valueForKey:@"tz"]];
        ldd2ldraw.offset = Vector3(tx.floatValue, -ty.floatValue, -tz.floatValue);

        NSNumber *ax = [mFormatter numberFromString:[attributeDict valueForKey:@"ax"]];
        NSNumber *ay = [mFormatter numberFromString:[attributeDict valueForKey:@"ay"]];
        NSNumber *az = [mFormatter numberFromString:[attributeDict valueForKey:@"az"]];
        ldd2ldraw.rotationAxis = Vector3(ax.floatValue, -ay.floatValue, -az.floatValue);

        ldd2ldraw.rotationAngle = [mFormatter numberFromString:[attributeDict valueForKey:@"angle"]].floatValue;
        [mTransformLU setValue:ldd2ldraw forKey:lego];
        if ( ![lego isEqualToString:ldraw] ) {
            [mTransformLU setValue:ldd2ldraw forKey:ldraw];
        }
//        NSLog(@"didStartElement --> %@", elementName);
    } else if ([elementName isEqualToString:@"Brick"] ) {
        //
        // <Brick ldraw="30237b.dat" lego="30237" />
        // transformation may follow
        //
        NSString *ldraw = [attributeDict valueForKey:@"ldraw"];
        ldraw = [ldraw stringByDeletingPathExtension];
        NSString *lego = [attributeDict valueForKey:@"lego"];
        LDD2LDRAW *ldd2ldraw = [mTransformLU valueForKey:lego];
        if ( ldd2ldraw == nil ) {
            ldd2ldraw = [[LDD2LDRAW alloc] init];
        }
        ldd2ldraw.ldr = ldraw;
        ldd2ldraw.ldd = lego;
        [mTransformLU setValue:ldd2ldraw forKey:lego];
        
        [mLdr2LddLU setValue:lego forKey:ldraw];
//        NSLog(@"didStartElement --> %@", elementName);
    } else {
//        NSLog(@"Ignore element [%@]", elementName);
    }
}

//-(void) parser:(NSXMLParser *)parser foundCharacters:(NSString *)string
//{
//    NSLog(@"foundCharacters --> %@", string);
//}
//
//- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName
//    namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
//{
//    NSLog(@"didEndElement   --> %@", elementName);
//}
//
//- (void) parserDidEndDocument:(NSXMLParser *)parser
//{
//    NSLog(@"parserDidEndDocument");
//}

- (LDD2LDRAW *)transformFor:(NSString *)lddID
{
    LDD2LDRAW *tr = [mTransformLU valueForKey:lddID];
    return tr;
}

- (long)ldrColorFor:(NSString *)lddColor
{
    NSString *ldrColorString = [mMaterialLU valueForKey:lddColor];
    long ldrColor = [mFormatter numberFromString:ldrColorString].integerValue;
    return ldrColor;
}

- (void)saveTransformDictionaryAs:(NSString *)path
{
//    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:mTransformLU, "transformLU", mMaterialLU, "materialsLU", mLdr2LddLU, "ldr2ldd", nil];
    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:3];
    [dict setObject:mTransformLU forKey:@"transformLU"];
    [dict setObject:mMaterialLU forKey:@"materialsLU"];
    [dict setObject:mLdr2LddLU forKey:@"ldr2ldd"];
    NSData *data = [NSKeyedArchiver archivedDataWithRootObject:dict];

    BOOL result = [data writeToFile:path atomically:YES];
    assert(result);
    
}

+ (LdrawXmlParser *)ldrawXmlFromFile:(NSString *)path
{
    NSData *data = [NSData dataWithContentsOfFile:path];
    if ( data == nil ) {
        return nil;
    }
    LdrawXmlParser *p = [[LdrawXmlParser alloc] init];
//    LdrawXmlParser *p = (LdrawXmlParser *)[NSKeyedUnarchiver unarchiveObjectWithData:data];

    NSDictionary *d = (NSDictionary *)[NSKeyedUnarchiver unarchiveObjectWithData:data];
    p->mTransformLU = [d objectForKey:@"transformLU"];
    p->mMaterialLU = [d objectForKey:@"materialsLU"];
    p->mLdr2LddLU = [d objectForKey:@"ldr2ldd"];
    return p;
}

@end

@implementation lddPart


@end

@interface lxfFile () {
    NSString *mMaterials;
    NSString *mLegoID;
    NSNumberFormatter *mFormatter;
    
    unzFile lxfZip;
    NSData *lxfData;
}

@end

@implementation lxfFile

+ (lxfFile *)lxfForPath:(NSString *)path
{
    lxfFile *file = [[lxfFile alloc] init];
    
    if ( [file parseFile:path] ) {
        return file;
    }
    return nil;
}

- (lxfFile *)init
{
    self = [super init];

    mFormatter = [[NSNumberFormatter alloc] init];
    mParts = [NSMutableArray array];
    return self;
}

- (bool)readZip:(NSString *)path
{
    lxfZip = unzOpen(path.UTF8String);
    if ( !lxfZip ) {
        NSLog(@"Fail to open zip %@", path);
        return false;
    }
    unz_file_info file_info;
    char filename_inzip[256];
    do {
        if (unzGetCurrentFileInfo(lxfZip, &file_info,
                                  filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0)) {
            NSLog(@"Fail to query file info in %@", path);
            return false;
        }
        if (strcasecmp(filename_inzip, "IMAGE100.LXFML") != 0) {
            unzGoToNextFile(lxfZip);
        } else {
            break;
        }
    } while (1);
    
    unsigned char *filebuff = new unsigned char [file_info.uncompressed_size];
    unzOpenCurrentFilePassword(lxfZip, NULL);
    if ( unzReadCurrentFile(lxfZip, filebuff, (unsigned int)file_info.uncompressed_size) < 0 ) {
        return false;
    }
    lxfData = [NSData dataWithBytes:filebuff length:file_info.uncompressed_size];
    unzCloseCurrentFile(lxfZip);
    delete [] filebuff;
    
    return true;
}

- (bool)parseFile:(NSString *)path
{
    NSError *error = nil;
    
    [self readZip:path];
    
    // Load the file and check the result
//    NSData *data = [NSData dataWithContentsOfFile:path
//        options:NSDataReadingUncached error:&error];
//    
//    if(error) {
//        NSLog(@"Error %@", error);
//        return false;
//    }
    
    NSXMLParser *parser = [[NSXMLParser alloc] initWithData:lxfData];
    parser.shouldProcessNamespaces = TRUE;
    [parser setDelegate:self];
    [parser parse];
    error = [parser parserError];
    
    if(error) {
        NSLog(@"Error %@", error);
        return false;
    }
    return true;
}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName
  namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
    attributes:(NSDictionary *)attributeDict
{
    if ( [elementName isEqualToString:@"Part"] ) {
        mLegoID = [attributeDict valueForKey:@"designID"];
        
        NSArray *materialsArray = [[attributeDict valueForKey:@"materials"] componentsSeparatedByString:@","];
        mMaterials = [materialsArray objectAtIndex:0];
    }

    if ( [elementName isEqualToString:@"Bone"] ) {
        //
        //
        // it looks like first 9 numbers is rotation matrix and last 3 are position
        //
        float transformCoeff[12];
        NSString *transform = [attributeDict valueForKey:@"transformation"];
        NSArray *transformArray = [transform componentsSeparatedByString:@","];
        for (int i = 0; i < 12; i++) {
            NSNumber *coeff = [mFormatter numberFromString:[transformArray objectAtIndex:i]];
            transformCoeff[i] = coeff.floatValue;
        }
//        Matrix44 LddTransform(Vector4(transformCoeff[0], -transformCoeff[3],  -transformCoeff[6],  0.0f),
//                              Vector4(-transformCoeff[1], transformCoeff[4],  transformCoeff[7],  0.0f),
//                              Vector4(-transformCoeff[2], transformCoeff[5],  transformCoeff[8],  0.0f),
//                              Vector4(transformCoeff[9], -transformCoeff[10], -transformCoeff[11], 1.0f)
//                              );
//        Matrix44 LddTransform(Vector4(transformCoeff[0], -transformCoeff[1],  -transformCoeff[2],  0.0f),
//                              Vector4(-transformCoeff[3], transformCoeff[4],  transformCoeff[5],  0.0f),
//                              Vector4(-transformCoeff[6], transformCoeff[7],  transformCoeff[8],  0.0f),
//                              Vector4(transformCoeff[9], -transformCoeff[10], -transformCoeff[11], 1.0f));
        Matrix33 LddRotation(Vector3(transformCoeff[0], -transformCoeff[1],  -transformCoeff[2]),
                             Vector3(-transformCoeff[3], transformCoeff[4],  transformCoeff[5]),
                             Vector3(-transformCoeff[6], transformCoeff[7],  transformCoeff[8]));
        Vector3 LddPosition(transformCoeff[9], -transformCoeff[10], -transformCoeff[11]);
        
        lddPart *newPart = [[lddPart alloc] init];
        newPart.legoID = mLegoID;
        newPart.rotation = LddRotation;
        newPart.position = LddPosition;
        newPart.color = mMaterials;
        [mParts addObject:newPart];
    }
}

- (void)exportTo:(NSString *)path withXml:(LdrawXmlParser *)withXml
{
    NSError *error = nil;
    NSOutputStream *os = [NSOutputStream outputStreamToFileAtPath:path append:FALSE];
    [os open];
    error = os.streamError;
    if(error) {
        NSLog(@"Error %@", error);
        return ;
    }

    for (lddPart *p in mParts) {
        LDD2LDRAW *tr = [withXml transformFor:p.legoID];
        if ( tr == nil ) {
            NSLog(@"No conversion of legoID %@", p.legoID);
            continue;
        }

        Matrix33 transformRotation = tr.Rotation;
        Vector3 rotatedOffset = lcMul(tr.offset, transformRotation);
        
        Vector3 rotatedToLxfOffset = lcMul(rotatedOffset, p.rotation);
        
//        Matrix33 newRotation = lcMul(transformRotation, p.rotation);
        Matrix33 newRotation = lcMul(p.rotation, transformRotation);
        
        
        Vector3 newOffset = p.position + rotatedToLxfOffset;

        //
        // 10 LDR = 0.4 LDD
        //
        newOffset *= 25;
        
//        printf("ldr line=[%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %@\n",
//               newOffset.x, newOffset.y, newOffset.z,
//               newRotation.r[0].x, newRotation.r[1].x, newRotation.r[2].x,
//               newRotation.r[0].y, newRotation.r[1].y, newRotation.r[2].y,
//               newRotation.r[0].z, newRotation.r[1].z, newRotation.r[2].z);
        long ldrColor = [withXml ldrColorFor:p.color];
        
        NSString *line = [NSString stringWithFormat:@"1 %ld %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %@.dat\n",
                          ldrColor,
                          newOffset.x, newOffset.y, newOffset.z,
                          newRotation.r[0].x, newRotation.r[1].x, newRotation.r[2].x,
                          newRotation.r[0].y, newRotation.r[1].y, newRotation.r[2].y,
                          newRotation.r[0].z, newRotation.r[1].z, newRotation.r[2].z,
                          tr.ldr];
        [os write:(const unsigned char *)line.UTF8String maxLength:line.length];
        error = os.streamError;
        if(error) {
            NSLog(@"Error %@", error);
            return ;
        }
    }
    [os close];
}

#if TARGET_OS_IOS
- (void)convertToLdrMesh:(std::map<std::string, std::vector<LEGO_PIECE_REF>> *)ldrModel withXml:(LdrawXmlParser *)withXml
{
    for (lddPart *p in mParts) {
        NSString *legoid = p.legoID;
        LDD2LDRAW *tr = [withXml transformFor:legoid];
        if ( tr == nil ) {
            NSLog(@"no transform for legoid=%@", legoid);
            continue;
        }
        //NSLog(@"transform for %@ ldd=%@ ldr=%@", legoid, tr.ldd, tr.ldr);

        Matrix33 transformRotation = tr.Rotation;
        Vector3 rotatedOffset = lcMul(tr.offset, transformRotation);
        
        Vector3 rotatedToLxfOffset = lcMul(rotatedOffset, p.rotation);
        
        Matrix33 newRotation = lcMul(p.rotation, transformRotation);
        
        Vector3 newOffset = p.position - rotatedToLxfOffset;
        
        //
        // 10 LDR = 0.4 LDD
        //
        newOffset *= 25;
        
        Matrix44 transform44 = Matrix44(newRotation, newOffset);
        
        int colorCode = (int)[withXml ldrColorFor:p.color];
        uint32_t rgbColor = colorForCode(colorCode);
        uint32_t complementColor = complementColorForCode(colorCode);
        
        Vector4 colorVec(LC_RGBA_RED(rgbColor), LC_RGBA_GREEN(rgbColor),
                         LC_RGBA_BLUE(rgbColor), LC_RGBA_ALPHA(rgbColor));
        Vector4 complementColorVec(LC_RGBA_RED(complementColor), LC_RGBA_GREEN(complementColor),
                                   LC_RGBA_BLUE(complementColor), LC_RGBA_ALPHA(complementColor));
        LEGO_PIECE_REF pieceRef = {
            .colorCode = colorCode,
            .hidden = false,
            .complementColorVec = complementColorVec,
            .colorVec = colorVec,
            .transform = transform44 };
        //ldrModel->push_back(pieceRef);
        std::vector<LEGO_PIECE_REF> &refList = (*ldrModel)[std::string(tr.ldr.UTF8String)];
        
        NSLog(@"adding ref %lu to %@ color {%.0f %.0f %.0f %.0f}",
                 refList.size(), tr.ldr, colorVec.x, colorVec.y, colorVec.z, colorVec.w);
        
        refList.push_back(pieceRef);
    }
}
#endif

@end





