#ifndef B_RENDERER_GL_H
#define B_RENDERER_GL_H

#include "OSdetect.h"

#ifdef B_OS_DESKTOP
	/* OpenGL imports handled by GLEW */
    #include <GL/glew.h>
	#include <GLFW/glfw3.h>
#endif
#ifdef B_OS_IOS
	/* Import OpenGL ES 3.0 */
	#include <OpenGLES/ES3/gl.h>
	#include <OpenGLES/ES3/glext.h>
#endif

#endif /* defined(B_RENDERER_GL_H) */