#include "PhotonBox/core/Display.h"

#include "PhotonBox/components/Camera.h"
#include "PhotonBox/resources/Texture.h"
#include "PhotonBox/resources/Resources.h"
#include "PhotonBox/core/FrameBuffer.h"

bool Display::_isRunning;
int Display::_width, Display::_height;
GLFWwindow* Display::_window;
bool Display::_isVSync;

void window_size_callback(GLFWwindow*, int, int);

void Display::init(const std::string& title, unsigned int width, unsigned int height, bool fullscreen, bool vsync)
{
	_width = width;
	_height = height;

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	_window = glfwCreateWindow(width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	if (!_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Set Icon
	GLFWimage images[1];
	images[0].pixels = Texture::loadIcon(Resources::ENGINE_RESOURCES + "/logo.png", images[0].width, images[0].height);
	glfwSetWindowIcon(_window, 1, images);

	setVSync(vsync);
	_isRunning = true;

	std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl << std::endl;

	glfwSetWindowSizeCallback(_window, window_size_callback);
}

void Display::destroy()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Display::setVSync(bool vsync)
{
	glfwSwapInterval((int)vsync);
	_isVSync = vsync;
}

void Display::setTitle(const std::string& title)
{
	glfwSetWindowTitle(_window, title.c_str());
}

void Display::swapBuffer()
{
	glfwSwapBuffers(_window);

	_isRunning = !glfwWindowShouldClose(_window);
}

unsigned int Display::getWidth()
{
	return _width;
}

unsigned int Display::getHeight()
{
	return _height;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	if (width == 0 || height == 0) return;

	Display::setRect(width, height);
	Camera::getMainCamera()->updateAspect();
	FrameBuffer::resizeAll();
}
