#ifndef DISPLAY_H
#define DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Display {
public:
	bool isRunning;

	~Display();

	void init(const std::string& title, unsigned int width, unsigned int height);
	void clearDisplay(float r, float b, float g, float a);
	void swapBuffer();
	void setRect(int width, int height) {
		_width = width;
		_height = height;
	}
	unsigned int getWidth();
	unsigned int getHeight();
private:
	int _width, _height;
	GLFWwindow* _window;
};

#endif /* defined(DISPLAY_H) */
