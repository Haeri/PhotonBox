#include "../header/Display.h"
#include <stdlib.h>

void Display::init(const std::string& title, unsigned int width, unsigned int height) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(1);
	isRunning = true;
}


Display::~Display() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Display::clearDisplay(float r, float b, float g, float a){
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(r, g, b, a);
}

void Display::swapBuffer() {
	glfwSwapBuffers(window);
	glfwPollEvents();

	isRunning = !glfwWindowShouldClose(window);
}