#include "../include/Time.h"

double Time::deltaTime;
double Time::time;

void Time::setTime(double _deltaTime, double _time) {
	deltaTime = _deltaTime;
	time = _time;
}