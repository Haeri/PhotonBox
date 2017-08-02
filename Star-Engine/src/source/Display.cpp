#include "../header/Display.h"
#include <stdlib.h>
#include "../header/Core.h"
#include "../header/Camera.h"

void window_size_callback(GLFWwindow* window, int width, int height) {
	Core::getInstance()->display.setRect(width, height);
	Camera::getMainCamera()->updateAspect();
}

void Display::init(const std::string& title, unsigned int width, unsigned int height) {
	_width = width;
	_height = height;
	
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!_window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(1);
	isRunning = true;


	glfwSetWindowSizeCallback(_window, window_size_callback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

Display::~Display() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Display::clearDisplay(float r, float b, float g, float a){
	float ratio;
	glfwGetFramebufferSize(_window, &_width, &_height);
	ratio = _width / (float)_height;
	glViewport(0, 0, _width, _height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(r, g, b, a);
}

void Display::swapBuffer() {
	glfwSwapBuffers(_window);
	glfwPollEvents();

	isRunning = !glfwWindowShouldClose(_window);
}

unsigned int Display::getWidth(){
	return _width;
}

unsigned int Display::getHeight(){
	return _height;
}
