#include "Time.h"
#include <chrono>

double Time::deltaTime;
double Time::time;
double Time::timeScale;

void Time::setDeltaTime(double deltaTime) {
	Time::deltaTime = deltaTime;
}

void Time::setTime(double time) {
	Time::time = time;
}

void Time::setTimeScale(double timeScale) {
	Time::timeScale = timeScale;
}

long Time::now() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
