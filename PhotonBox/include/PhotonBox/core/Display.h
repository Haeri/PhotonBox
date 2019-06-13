#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

#include "PhotonBox/core/ISystem.h"
#include "PhotonBox/core/OpenGL.h"

class Display : public ISystem
{
public:
	static bool isRunning() { return _isRunning; }
	static GLFWwindow* getWindow() { return _window; }
	static void setVSync(bool vsync);
	static void setTitle(const std::string& title);
	static void swapBuffer();
	static void setRect(int width, int height)
	{
		_width = width;
		_height = height;
	}
	static unsigned int getWidth();
	static unsigned int getHeight();

	void init(Config::Profile profile) override;
	void start() override {}
	void destroy() override;
private:
	static bool _isRunning;
	static int _width, _height;
	static GLFWwindow* _window;
	static bool _isVSync;
};

#endif // DISPLAY_H
