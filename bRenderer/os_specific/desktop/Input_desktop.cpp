#include "headers/OSdetect.h"

#ifdef B_OS_DESKTOP

#include "headers/Input.h"

Input::Input()
{}

Input::Input(ViewPtr view)
	: _view(view)
{}

Input::~Input()
{}

GLint Input::getKeyState(GLint key)
{
	return glfwGetKey(_view->getWindow(), key);
}

GLint Input::getMouseButtonState(GLint button)
{
	return glfwGetMouseButton(_view->getWindow(), button);
}

double Input::getCursorPositionX()
{
	double xpos, ypos;
	getCursorPosition(&xpos, &ypos);
	return xpos;
}

double Input::getCursorPositionY()
{
	double xpos, ypos;
	getCursorPosition(&xpos, &ypos);
	return ypos;
}

void Input::getCursorPosition(double* x, double* y, bool* deviceHasCursor)
{
	glfwGetCursorPos(_view->getWindow(), x, y);
	if (deviceHasCursor != nullptr){
		*deviceHasCursor = true;
	}
}

double   Input::getGyroscopePitch(bool* deviceHasGyroscope)
{
	if (deviceHasGyroscope != nullptr){
		*deviceHasGyroscope = false;
	}
	return 0.0;
}

double   Input::getGyroscopeYaw(bool* deviceHasGyroscope)
{
	if (deviceHasGyroscope != nullptr){
		*deviceHasGyroscope = false;
	}
	return 0.0;
}

double   Input::getGyroscopeRoll(bool* deviceHasGyroscope)
{
	if (deviceHasGyroscope != nullptr){
		*deviceHasGyroscope = false;
	}
	return 0.0;
}

TouchMap Input::getTouches()
{
	return TouchMap();
}

bool Input::singleTapRecognized()
{
	return false;
}

bool Input::doubleTapRecognized()
{
	return false;
}

Touch Input::getLastSingleTapLocation()
{
	return { 0.0f, 0.0f, 0.0f, 0.0f };
}

Touch Input::getLastDoubleTapLocation()
{
	return{ 0.0f, 0.0f, 0.0f, 0.0f };
}

void Input::setCursorEnabled()
{
	glfwSetInputMode(_view->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Input::setCursorEnabled(bool enabled)
{
	enabled ? setCursorEnabled() : setCursorDisabled();
}

void Input::setCursorDisabled()
{
	glfwSetInputMode(_view->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::setCursorPosition(double x, double y)
{
	glfwSetCursorPos(_view->getWindow(), x, y);
}

bool Input::isTouchDevice()
{
	return false;
}

#endif