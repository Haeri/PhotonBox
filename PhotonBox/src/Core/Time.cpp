#include "Time.h"
#include <chrono>

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

long Time::now()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
