#include "PhotonBox/core/Time.h"

#include "PhotonBox/core/OpenGL.h"

double Time::deltaTime = 0.001;
float Time::deltaTimef = 0.001f;
double Time::time;
double Time::timeScale;

void Time::setDeltaTime(double deltaTime)
{
	Time::deltaTime = deltaTime;
	Time::deltaTimef = (float)deltaTime;
}

void Time::setTime(double time)
{
	Time::time = time;
}

void Time::setTimeScale(double timeScale)
{
	Time::timeScale = timeScale;
}

double Time::now()
{
	return glfwGetTime();
}
