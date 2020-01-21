//
//  RenderToImage.h
//  simpleGL
//
//  Created by Leonid on 26/09/2015.
//  Copyright © 2015 lFroen. All rights reserved.
//

#ifndef RenderToImage_h
#define RenderToImage_h

#include "legomesh.h"

#define USE_MSAA 1

@interface ModelRenderToImage : NSObject
{
    EAGLContext* myContext;
    GLuint sampleFramebuffer, sampleColorRenderbuffer, sampleDepthbuffer;
    
    GLuint framebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    
    int width;
    int height;
    GLubyte *data;
}

- (id) initWithSize:(CGSize)size shareGroup:(EAGLSharegroup *)shareGroup;

-(UIImage *) renderModel:(LegoModel *)model withPiece:(LegoPiece *)withPiece withRef:(LEGO_PIECE_REF *)withRef;

@end


@interface LegoImageLib : NSObject {
    NSMutableDictionary *mSmallImages;
}

- (id) initWithSharegroup:(EAGLSharegroup *)shareGroup;

- (void) generateForModel:(LegoModel *)model;

@end

#endif /* RenderToImage_h */
