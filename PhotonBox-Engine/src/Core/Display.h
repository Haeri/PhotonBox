#ifndef DISPLAY_H
#define DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Display {
public:
	static bool isRunning() { return _isRunning; }
	static GLFWwindow* getWindow() { return _window; }
	static void clearDisplay(float r, float b, float g, float a);
	static void swapBuffer();
	static void setRect(int width, int height) {
		_width = width;
		_height = height;
	}
	static unsigned int getWidth();
	static unsigned int getHeight();

	void init(const std::string& title, unsigned int width, unsigned int height);
	void destroy();
private:
	static bool _isRunning;
	static int _width, _height;
	static GLFWwindow* _window;
};

#endif // DISPLAY_H
