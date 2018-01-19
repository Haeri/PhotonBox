#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include "OpenGL.h"

class Display
{
public:
	static bool isRunning() { return _isRunning; }
	static GLFWwindow* getWindow() { return _window; }
	static void setVSync(bool vsync);
	static void swapBuffer();
	static void setRect(int width, int height)
	{
		_width = width;
		_height = height;
	}
	static unsigned int getWidth();
	static unsigned int getHeight();

	void init(const std::string& title, unsigned int width, unsigned int height, bool fullscreen, bool vsync);
	void destroy();
private:
	static bool _isRunning;
	static int _width, _height;
	static GLFWwindow* _window;
	static bool _isVSync;
};

#endif // DISPLAY_H
