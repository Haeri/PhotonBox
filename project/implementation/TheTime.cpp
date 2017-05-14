#include "TheTime.h"

double TheTime::deltaTime;
double TheTime::time;

void TheTime::setTime(double _deltaTime, double _time) {
	deltaTime = _deltaTime;
	time = _time;
}