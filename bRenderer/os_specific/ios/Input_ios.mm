#include "headers/OSdetect.h"
#ifdef B_OS_IOS

#include "headers/Input.h"

Input::Input()
{
    _motionManager = [[CMMotionManager alloc] init];
    _motionManager.deviceMotionUpdateInterval = 1.0f/60.0f;
    [_motionManager startDeviceMotionUpdates];
}

Input::Input(ViewPtr view)
    : _view(view)
{
    _motionManager = [[CMMotionManager alloc] init];
    _motionManager.deviceMotionUpdateInterval = 1.0f/60.0f;
    [_motionManager startDeviceMotionUpdates];
}

Input::~Input()
{
    _motionManager = nil;
}

GLint Input::getKeyState(GLint key)
{
	return bRenderer::INPUT_UNDEFINED;
}

GLint Input::getMouseButtonState(GLint button)
{
	return bRenderer::INPUT_UNDEFINED;
}

double Input::getCursorPositionX()
{
	return 0.0;
}

double Input::getCursorPositionY()
{
	return 0.0;
}

void Input::getCursorPosition(double* x, double* y, bool* deviceHasCursor)
{
	if (deviceHasCursor != nullptr){
		*deviceHasCursor = false;
	}
}

double   Input::getGyroscopePitch(bool* deviceHasGyroscope)
{
	if (deviceHasGyroscope != nullptr){
		*deviceHasGyroscope = true;
	}
	return _motionManager.deviceMotion.attitude.pitch;
}

double   Input::getGyroscopeYaw(bool* deviceHasGyroscope)
{
	if (deviceHasGyroscope != nullptr){
		*deviceHasGyroscope = true;
	}
	return _motionManager.deviceMotion.attitude.yaw;
}

double   Input::getGyroscopeRoll(bool* deviceHasGyroscope)
{
	if (deviceHasGyroscope != nullptr){
		*deviceHasGyroscope = true;
	}
	return _motionManager.deviceMotion.attitude.roll;
}

TouchMap Input::getTouches()
{
    return [_view->getUIView() getTouches];
}

bool Input::singleTapRecognized()
{
    return [_view->getUIView() singleTapRecognized];
}

bool Input::doubleTapRecognized()
{
    return [_view->getUIView() doubleTapRecognized];
}

Touch Input::getLastSingleTapLocation()
{
    return [_view->getUIView() getLastSingleTapLocation];
}

Touch Input::getLastDoubleTapLocation()
{
    return [_view->getUIView() getLastDoubleTapLocation];
}

void Input::setCursorEnabled()
{

}

void Input::setCursorEnabled(bool enabled)
{

}

void Input::setCursorDisabled()
{

}

void Input::setCursorPosition(double x, double y)
{

}

bool Input::isTouchDevice()
{
	return true;
}

#endif