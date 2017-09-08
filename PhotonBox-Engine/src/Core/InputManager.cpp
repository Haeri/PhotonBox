#include "InputManager.h"
#include <iostream>
#include <algorithm>
#include "Time.h"

std::vector<int> InputManager::_keyPress;
std::vector<int> InputManager::_keyRelease;
double InputManager::_xPos = 0;
double InputManager::_yPos = 0;
double InputManager::_xPosLast = 0;
double InputManager::_yPosLast = 0;

void key_callback(GLFWwindow*, int, int, int, int);
void cursor_position_callback(GLFWwindow*, double, double);

void InputManager::pollEvents() {
	glfwPollEvents();
}

void InputManager::init() {
	_xPos = 0;
	_yPos = 0;
	_xPosLast = 0;
	_yPosLast = 0;
	glfwSetInputMode(Display::getWindow(), GLFW_STICKY_KEYS, 1);
	glfwSetKeyCallback(Display::getWindow(), key_callback);
	glfwSetCursorPosCallback(Display::getWindow(), cursor_position_callback);
}

bool InputManager::keyPressed(int key) {
	for (size_t i = 0; i < _keyPress.size(); ++i) {
		if (_keyPress[i] == key) return true;
	}
	return false;
}

bool InputManager::keyReleased(int key) {
	for (size_t i = 0; i < _keyRelease.size(); ++i) {
		if (_keyRelease[i] == key) return true;
	}
	return false;
}

Vector2f InputManager::getMouse(){
	float x = (_xPos - Display::getWidth() / 2.0f) / Display::getWidth() * 2.0f;
	float y = (_yPos - Display::getHeight() / 2.0f) / Display::getHeight() * -2.0f;
	return Vector2f(x, y);
}

Vector2f InputManager::getMouseDelta() {
	float xl = (_xPosLast - Display::getWidth() / 2.0f) / Display::getWidth() * 2.0f;
	float yl = (_yPosLast - Display::getHeight() / 2.0f) / Display::getHeight() * -2.0f;
	float x = (_xPos - Display::getWidth() / 2.0f) / Display::getWidth() * 2.0f;
	float y = (_yPos - Display::getHeight() / 2.0f) / Display::getHeight() * -2.0f;
	float xres = (x - xl) / (Time::deltaTime == 0? 0.001 : Time::deltaTime);
	float yres = (y - yl) / (Time::deltaTime == 0 ? 0.001 : Time::deltaTime);
	return Vector2f(xres, yres);
}

void InputManager::setCursorMode(CursorMode mode){
	glfwSetInputMode(Display::getWindow(), GLFW_CURSOR, (int)mode);
}

void InputManager::handlePress(int key) {
	_keyPress.push_back(key);
}

void InputManager::handleRelease(int key) {
	_keyPress.erase(std::remove(_keyPress.begin(), _keyPress.end(), key), _keyPress.end());
	_keyRelease.push_back(key);
}

void InputManager::handleMouse(double x, double y) {
	_xPos = x;
	_yPos = y;
}

void InputManager::update() {
	_xPosLast = _xPos;
	_yPosLast = _yPos;
	_keyRelease.clear();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS){
		InputManager::handlePress(key);
	}else if (action == GLFW_RELEASE) {
		InputManager::handleRelease(key);
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
	InputManager::handleMouse(xpos, ypos);
}