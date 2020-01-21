//
//  RenderToImage.m
//  simpleGL
//
//  Created by Leonid on 26/09/2015.
//  Copyright © 2015 lFroen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/ES2/glext.h>
#import <GLKit/GLKit.h>

#import "ModelRenderToImage.h"

#include "multifile.h"
#include "legomesh.h"
#include "ldrawcolor.h"

#define CHECK_GL_ERRORS() \
do {                                                                                          \
GLenum error = glGetError();                                                                  \
if(error != GL_NO_ERROR) {                                                                    \
NSLog(@"OpenGL(objc): %s in %s:%d [error %d]", __FUNCTION__, __FILE__, __LINE__, (int)error); \
\
assert(0); \
} \
} while(false)


@interface ModelRenderToImage () {
    LdrawRender *mLdrawRender;
    LdrawLib *mLdrawLib;
}
@end


@implementation ModelRenderToImage

- (id) initWithSize:(CGSize)size shareGroup:(EAGLSharegroup *)shareGroup
{
    self = [super init];
    if (self) {
        width = size.width;
        height = size.height;
        NSInteger dataLength = width * height * 4;
        data = (GLubyte*)malloc(dataLength * sizeof(GLubyte));
        [self setupOpenGL: shareGroup];
        mLdrawLib = LdrawLib::getSharedInstance();
        mLdrawRender = new LdrawRender();
        mLdrawRender->loadDefaultShaders();
        mLdrawRender->setProjectMatrix(width, height);
        mLdrawRender->initDraw();

        [EAGLContext setCurrentContext:myContext];
    }
    return self;
}


-(void) setupOpenGL:(EAGLSharegroup *)shareGroup
{
    myContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:shareGroup];
    [EAGLContext setCurrentContext:myContext];
    
    glGenFramebuffers(1, &sampleFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, sampleFramebuffer);
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"failed to make complete framebuffer object %x", status);
        abort();
    }

    //
    // if multisample
    //
#if USE_MSAA
    glGenFramebuffers(1, &sampleFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, sampleFramebuffer);

    glGenRenderbuffers(1, &sampleColorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, sampleColorRenderbuffer);
    glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_RGBA8_OES, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, sampleColorRenderbuffer);

    glGenRenderbuffers(1, &sampleDepthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, sampleDepthbuffer);
    glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, sampleDepthbuffer);

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"failed to make complete framebuffer object %x", status);
        abort();
    }
#endif
}



-(UIImage *) renderModel:(LegoModel *)model withPiece:(LegoPiece *)withPiece withRef:(LEGO_PIECE_REF *)pieceRef
{
    [EAGLContext setCurrentContext:myContext];
    glViewport(0, 0, width, height);
    
#if USE_MSAA
    glBindFramebuffer(GL_FRAMEBUFFER, sampleFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, sampleColorRenderbuffer);
#else
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
#endif
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mLdrawRender->setModel(model);
    
    mLdrawRender->setProjectMatrix(width, height);
    mLdrawRender->initViewMatrix(&withPiece->Bounds());
    
    mLdrawRender->renderFrame(withPiece, *pieceRef, false);
    
#if USE_MSAA
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, framebuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, sampleFramebuffer);
    glResolveMultisampleFramebufferAPPLE();
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
#endif

// grabbing image from FBO
//    GLint backingWidth, backingHeight;
//    
//    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
//    
//    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    GLint x = 0, y = 0;
    NSInteger dataLength = width * height * 4;
    
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    
    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, data, dataLength, NULL);
    
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
    
    CGImageRef iref = CGImageCreate(width, height, 8, 32, width * 4, colorspace,
                                    kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast,
                                    ref, NULL, true, kCGRenderingIntentDefault);
    
    UIGraphicsBeginImageContext(CGSizeMake(width, height));
    
    CGContextRef cgcontext = UIGraphicsGetCurrentContext();
    CGContextSetBlendMode(cgcontext, kCGBlendModeCopy);
    CGContextDrawImage(cgcontext, CGRectMake(0.0, 0.0, width, height), iref);
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();

    CFRelease(ref);
    CFRelease(colorspace);
    CGImageRelease(iref);
    
    [EAGLContext setCurrentContext:nil];
    return image;
}

@end
