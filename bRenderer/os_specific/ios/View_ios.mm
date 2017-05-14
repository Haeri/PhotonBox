#include "headers/OSdetect.h"
#ifdef B_OS_IOS

#include "headers/View.h"
#include <vector>

/* Constructor and destructor */
View::View()
	: _initialized(false)
{}

View::~View()
{
    if (_initialized)
		terminateView();
}

/* Public functions */

bool View::initView(std::string windowTitle)
{
    if (_initialized)
        return true;
    
    bRenderer::log("Initializing view", bRenderer::LM_SYS);
    
    CGRect frame = [[UIScreen mainScreen] applicationFrame];
    _view = [[BView alloc] initWithFrame:frame];
    
    _fullscreen = true;

	// OpenGL
	// clear
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	clearScreen();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    _initialized = true;
    
    return true;
}

bool View::initView(bool fullscreen, std::string windowTitle)
{
    if (_initialized)
        return true;
    
    if (!fullscreen)
        bRenderer::log("Can't set fullscreen to flase without setting width and height on iOS.", bRenderer::LM_WARNING);
    
    return initView(windowTitle);
}

bool View::initView(GLint width, GLint height, bool fullscreen, std::string windowTitle)
{
    if (_initialized)
        return true;
    
    initView(windowTitle);
   
    if (fullscreen){
        bRenderer::log("Can't set width and height when using fullscreen on iOS.", bRenderer::LM_WARNING);
    }
    else{
        [_view setViewWidth:width setViewHeight:height];
        _fullscreen = false;
    }
    
    return true;
}

void View::terminateView()
{
    [_view removeFromSuperview];
    _view = nil;

	_initialized = false;
}

bool View::isInitialized()
{
    return _initialized;
}

bool View::isRunning()
{
    return _initialized;
}

bool View::isFullscreen()
{
    return _fullscreen;
}

GLint View::getWidth()
{
    return [_view getViewWidth];
}

GLint View::getHeight()
{
    return [_view getViewHeight];
}

void View::getSize(GLint* width, GLint* height)
{
    *width = getWidth();
    *height = getHeight();
}

GLint View::getScreenWidth()
{
	return [[UIScreen mainScreen] bounds].size.width;
}

GLint View::getScreenHeight()
{
	return [[UIScreen mainScreen] bounds].size.height;
}

void View::getScreenSize(GLint* width, GLint* height)
{
	*width = getScreenWidth();
	*height = getScreenHeight();
}

GLint View::getViewportWidth()
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	return vp[2];
}

GLint View::getViewportHeight()
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	return vp[3];
}

void View::getViewportSize(GLint* width, GLint* height)
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	*width = vp[2];
	*height = vp[3];
}

GLfloat View::getAspectRatio()
{
    return (GLfloat)getWidth() / (GLfloat)getHeight();
}

GLint View::getPositionX()
{
    return [_view getViewPositionX];
}

GLint View::getPositionY()
{
    return [_view getViewPositionY];
}

void View::getPosition(GLint* x, GLint* y)
{
    *x = getPositionX();
    *y = getPositionY();
}

View::GLFWwindow* View::getWindow()
{
    return 0;
}

BView* View::getUIView()
{
    return _view;
}

void View::attachToUIView(UIView* view)
{
    [_view removeFromSuperview];
    [view addSubview:_view];
    
    if(_fullscreen)
        [_view setFullscreen];
}

void View::setFullscreen(bool fullscreen)
{
    if(fullscreen){
        [_view setFullscreen];
        _fullscreen = true;
    }
    else
        bRenderer::log("Set width and height to set fullscreen to flase", bRenderer::LM_WARNING);
}


void View::setWidth(GLint width)
{
    [_view setViewWidth:width setViewHeight:getHeight()];
    _fullscreen = false;
}

void View::setHeight(GLint height)
{
    [_view setViewWidth:getWidth() setViewHeight:height];
    _fullscreen = false;
}

void View::setSize(GLint width, GLint height)
{
    [_view setViewWidth:width setViewHeight:height];
    _fullscreen = false;
}

void View::setViewportSize(GLint width, GLint height)
{
	glViewport(0, 0, width, height);
}


void View::setPosition(GLint x, GLint y)
{
    [_view setViewPositionX:x setViewPositionY:y];
}

void View::setContextCurrent()
{
	[_view setContextCurrent];
}

void View::bindFramebuffer()
{
    [_view bindFramebuffer];
}

void View::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void View::presentBuffer()
{
    [_view presentRenderbuffer];
}

static void windowSizeChanged(View::GLFWwindow* window, int width, int height)
{
    // Not implemented on iOS
}

#endif