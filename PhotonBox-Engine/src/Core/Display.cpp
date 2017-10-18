#include "Display.h"
#include "../Components/Camera.h"
#include "../Resources/Texture.h"

bool Display::_isRunning;
int Display::_width, Display::_height;
GLFWwindow* Display::_window;
bool Display::_isVSync;

void window_size_callback(GLFWwindow*, int, int);

void Display::init(const std::string& title, unsigned int width, unsigned int height) {
	_width = width;
	_height = height;
	
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);


	_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!_window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Set Icon
	GLFWimage images[1];
	images[0].pixels = Texture::loadIcon("./res/PhotonBox.png", images[0].width, images[0].height);
	glfwSetWindowIcon(_window, 1, images);

	setVSync(false);
	_isRunning = true;

	std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl << std::endl;

	glfwSetWindowSizeCallback(_window, window_size_callback);
}

void Display::destroy() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Display::clearDisplay(float r, float b, float g, float a){
	glfwGetFramebufferSize(_window, &_width, &_height);
	//glViewport(0, 0, _width, _height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(r, g, b, a);
}

void Display::setVSync(bool vsync) {
	glfwSwapInterval((int)vsync);
	_isVSync = vsync;
}

void Display::swapBuffer() {
	glfwSwapBuffers(_window);

	_isRunning = !glfwWindowShouldClose(_window);
}

unsigned int Display::getWidth(){
	return _width;
}

unsigned int Display::getHeight(){
	return _height;
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	Display::setRect(width, height);
	Camera::getMainCamera()->updateAspect();
}
