#include "PhotonBox/core/Time.h"

#include "PhotonBox/core/OpenGL.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

double Time::deltaTime = 0.001;
float Time::deltaTimef = 0.001f;
double Time::time = 0.0;
float Time::timeScale = 1.0f;

void Time::setDeltaTime(double deltaTime)
{
	Time::deltaTime = deltaTime * timeScale;
	Time::deltaTimef = (float)deltaTime * timeScale;
	time += deltaTime;
}

void Time::setTime(double time)
{
	Time::time = time;
}

void Time::setTimeScale(float timeScale)
{
	Time::timeScale = timeScale;
}

double Time::now()
{
	return glfwGetTime();
}
