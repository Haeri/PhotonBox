#ifndef B_BVIEW_H
#define B_BVIEW_H

#include "headers/OSdetect.h"
#ifdef B_OS_IOS

#import <UIKit/UIKit.h>

#include "headers/Renderer_GL.h"
#include "Touch.h"

@class EAGLContext;

/* This is a UIView specifically designed for bRenderer and needs to be added to a window in your project */
@interface BView : UIView
{
	@private
    GLint _width;
	GLint _height;

	GLuint _defaultFramebuffer, _colorRenderbuffer, _depthRenderbuffer;

	// The display link is used to create a render loop
	CAEAGLLayer *_eaglLayer;
    
    // OpenGL context
    EAGLContext *_context;
    
    // Touch handling
    bool _singleTapRecognized;
    bool _doubleTapRecognized;
    CGPoint _lastDoubleTapLocation;
    CGPoint _lastSingleTapLocation;
    TouchMap _touches;
    
}

-(void)bindFramebuffer;
-(void)presentRenderbuffer;
-(bool)setContextCurrent;

-(int)getViewWidth;
-(int)getViewHeight;
-(int)getViewPositionX;
-(int)getViewPositionY;

-(void)setFullscreen;
-(void)setViewWidth:(GLint)w setViewHeight : (GLint)h;
-(void)setViewPositionX:(GLint)x setViewPositionY : (GLint)y;


-(TouchMap)getTouches;
-(bool)singleTapRecognized;
-(bool)doubleTapRecognized;
-(Touch)getLastSingleTapLocation;
-(Touch)getLastDoubleTapLocation;

@end

#endif

#endif /* defined(B_BVIEW_H) */