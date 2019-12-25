#include "PhotonBox/core/InputManager.h"

#include <iostream>
#include <algorithm>

#include "PhotonBox/core/Time.h"
#include "PhotonBox/core/Display.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

std::vector<int> InputManager::_keyPress;
std::vector<int> InputManager::_keyDown;
std::vector<int> InputManager::_keyRelease;
float InputManager::_xPos = 0;
float InputManager::_yPos = 0;
float InputManager::_xPosLast = 0;
float InputManager::_yPosLast = 0;

void key_callback(GLFWwindow*, int, int, int, int);
void cursor_position_callback(GLFWwindow*, double, double);

void InputManager::pollEvents()
{
	glfwPollEvents();
}

void InputManager::init(Config::Profile profile)
{
	std::cout << "Initializing InputManager\n";
	_xPos = 0;
	_yPos = 0;
	_xPosLast = 0;
	_yPosLast = 0;
	glfwSetInputMode(Display::getWindow(), GLFW_STICKY_KEYS, 1);
	glfwSetKeyCallback(Display::getWindow(), key_callback);
	glfwSetCursorPosCallback(Display::getWindow(), cursor_position_callback);
}

bool InputManager::keyPressed(int key)
{
	for (size_t i = 0; i < _keyPress.size(); ++i)
	{
		if (_keyPress[i] == key) return true;
	}
	return false;
}

bool InputManager::keyDown(int key)
{
	for (size_t i = 0; i < _keyDown.size(); ++i)
	{
		if (_keyDown[i] == key) return true;
	}
	return false;
}

bool InputManager::keyReleased(int key)
{
	for (size_t i = 0; i < _keyRelease.size(); ++i)
	{
		if (_keyRelease[i] == key) return true;
	}
	return false;
}

Vector2f InputManager::getMouse()
{
	float x = (_xPos - Display::getWidth() / 2.0f) / Display::getWidth() * 2.0f;
	float y = (_yPos - Display::getHeight() / 2.0f) / Display::getHeight() * -2.0f;
	return Vector2f(x, y);
}

Vector2f InputManager::getMouseDelta()
{
	float xl = (_xPosLast - Display::getWidth() / 2.0f) / Display::getWidth() * 2.0f;
	float yl = (_yPosLast - Display::getHeight() / 2.0f) / Display::getHeight() * -2.0f;
	float x = (_xPos - Display::getWidth() / 2.0f) / Display::getWidth() * 2.0f;
	float y = (_yPos - Display::getHeight() / 2.0f) / Display::getHeight() * -2.0f;
	float xres = (x - xl) / (Time::deltaTimef == 0 ? 0.001f : Time::deltaTimef);
	float yres = (y - yl) / (Time::deltaTimef == 0 ? 0.001f : Time::deltaTimef);
	return Vector2f(xres, yres);
}

void InputManager::setCursorMode(CursorMode mode)
{
	glfwSetInputMode(Display::getWindow(), GLFW_CURSOR, (int)mode);
}

InputManager::CursorMode InputManager::getCursorMode()
{
	return (CursorMode)glfwGetInputMode(Display::getWindow(), GLFW_CURSOR);
}

void InputManager::handleKey(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		_keyPress.push_back(key);
		_keyDown.push_back(key);
	}
	else if (action == GLFW_RELEASE)
	{
		_keyDown.erase(std::remove(_keyDown.begin(), _keyDown.end(), key), _keyDown.end());
		_keyRelease.push_back(key);
	}
}

void InputManager::handleMouse(float x, float y)
{
	_xPos = x;
	_yPos = y;
}

void InputManager::update()
{
	_xPosLast = _xPos;
	_yPosLast = _yPos;
	_keyPress.clear();
	_keyRelease.clear();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputManager::handleKey(key, action);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	InputManager::handleMouse((float)xpos, (float)ypos);
}
