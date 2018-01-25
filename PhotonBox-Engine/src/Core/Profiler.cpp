#include "Profiler.h"

#include <limits>
#include <iostream>

int Profiler::_minFPS = std::numeric_limits<int>::max();;
int Profiler::_maxFPS;
float Profiler::_avgFPS;
unsigned long int Profiler::_allFrames;
unsigned long int Profiler::_allSeconds;

void Profiler::addFps(int fps)
{
	if (_allSeconds++ == WARMUP_SECONDS) { startProfiling(); }

	if (fps > _maxFPS) _maxFPS = fps;
	if (fps < _minFPS) _minFPS = fps;
	_avgFPS = ((float)((_allFrames - 1.0) * _avgFPS) + (float)fps) / (float)(++_allFrames);
}

void Profiler::reset() {
	_minFPS = std::numeric_limits<int>::max();;
	_maxFPS = 0;
	_avgFPS = 0;
	_allFrames = 0;
	_allSeconds = 0;
}

void Profiler::startProfiling()
{
	_minFPS = std::numeric_limits<int>::max();;
	_maxFPS = 0;
	_avgFPS = 0;
	_allFrames = 0;
}
