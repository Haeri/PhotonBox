#include "headers/OSdetect.h"

#ifdef B_OS_DESKTOP

#include "headers/View.h"

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
	return initView(_fullscreen, windowTitle);
}

bool View::initView(bool fullscreen, std::string windowTitle)
{
	if (fullscreen)
		return initView(getScreenWidth(), getScreenHeight(), fullscreen, windowTitle);
	else
		return initView(bRenderer::DEFAULT_VIEW_WIDTH(), bRenderer::DEFAULT_VIEW_HEIGHT(), fullscreen, windowTitle);
}

bool View::initView(GLint width, GLint height, bool fullscreen, std::string windowTitle)
{
	if (_initialized)
		return true;

	bRenderer::log("Initializing view", bRenderer::LM_SYS);

	// Initialize GLFW 
	if (!glfwInit())
		return false;

	// Create a windowed mode window and its OpenGL context 
	_window = glfwCreateWindow(width, height, windowTitle.c_str(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	_fullscreen = fullscreen;

	if (!_window)
	{
		glfwTerminate();
		return false;
	}

	// Make the window's context current 
	glfwMakeContextCurrent(_window);

	// Add resize handler 
	glfwSetWindowSizeCallback(_window, windowSizeChanged);

	bRenderer::log("Window and Context created", bRenderer::LM_SYS);

	// Initialize GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Problem: glewInit failed, something is seriously wrong. 
		bRenderer::log(glewGetErrorString(err), bRenderer::LM_ERROR);
		return false;
	}
	bRenderer::log("Using GLEW ", glewGetString(GLEW_VERSION), bRenderer::LM_SYS);

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

void View::terminateView()
{
	glfwDestroyWindow(_window);
	_initialized = false;
}

bool View::isInitialized()
{
	return _initialized;
}

bool View::isRunning()
{
	return !glfwWindowShouldClose(_window);
}

bool View::isFullscreen()
{
	return _fullscreen;
}

GLint View::getWidth()
{
	int w, h;
	getSize(&w, &h);
	return w;
}

GLint View::getHeight()
{
	int w, h;
	getSize(&w, &h);
	return h;
}

void View::getSize(GLint* width, GLint* height)
{
	glfwGetWindowSize(_window, width, height);
}

GLint View::getScreenWidth()
{
	glfwInit();
	return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
}

GLint View::getScreenHeight()
{
	glfwInit();
	return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
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
	int w, h;
	getSize(&w, &h);
	return (GLfloat)w / (GLfloat)h;
}

GLint View::getPositionX()
{
	int x, y;
	getPosition(&x, &y);
	return x;
}

GLint View::getPositionY()
{
	int x, y;
	getPosition(&x, &y);
	return y;
}

void View::getPosition(GLint* x, GLint* y)
{
	glfwGetWindowPos(_window, x, y);
}

GLFWwindow* View::getWindow()
{
	return _window;
}

View::BView* View::getUIView()
{
	return 0;
}

void View::attachToUIView(View::UIView* view){}

void View::setFullscreen(bool fullscreen)
{
	bRenderer::log("Setting full screen after initialization is not yet supported on desktop systems", bRenderer::LM_WARNING);
}


void View::setWidth(GLint width)
{
	glfwSetWindowSize(_window, width, getHeight());
}

void View::setHeight(GLint height)
{
	glfwSetWindowSize(_window, getWidth(), height);
}

void View::setSize(GLint width, GLint height)
{
	glfwSetWindowSize(_window, width, height);
}

void View::setViewportSize(GLint width, GLint height)
{
	glViewport(0, 0, width, height);
}

void View::setPosition(GLint x, GLint y)
{
	glfwSetWindowPos(_window, x, y);
}

void View::setContextCurrent()
{
	glfwMakeContextCurrent(_window);
}

void View::bindFramebuffer()
{

}

void View::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.5, 0.5, 0.5, 1);
}

void View::presentBuffer()
{
	glfwSwapBuffers(_window);
}

/* Private static functions */

void View::windowSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	bRenderer::log("width: " + std::to_string(width)+", height: " + std::to_string(height), bRenderer::LM_SYS);
}


#endif