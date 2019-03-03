#include "PhotonBox/core/Profiler.h"

#include <limits>
#include <iostream>
#include <string>

#include "PhotonBox/core/system/DebugGUI.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

int Profiler::_minFPS = std::numeric_limits<int>::max();;
int Profiler::_maxFPS;
float Profiler::_avgFPS;
unsigned long int Profiler::_allFrames;
unsigned long int Profiler::_allSeconds;
MaxQueue<float> Profiler::_frames(FRAME_WINDOW_SIZE, true);

void Profiler::addFps(int fps)
{
	if (_allSeconds++ == WARMUP_SECONDS) { startProfiling(); }

	if (fps > _maxFPS) _maxFPS = fps;
	if (fps < _minFPS) _minFPS = fps;
	_avgFPS = ((float)((_allFrames - 1.0) * _avgFPS) + (float)fps) / (float)(++_allFrames);

	_frames.add(static_cast<float>(fps));
}

void Profiler::reset() {
	_minFPS = std::numeric_limits<int>::max();;
	_maxFPS = 0;
	_avgFPS = 0;
	_allFrames = 0;
	_allSeconds = 0;

	_frames.clear();
}

void Profiler::startProfiling()
{
	_minFPS = std::numeric_limits<int>::max();;
	_maxFPS = 0;
	_avgFPS = 0;
	_allFrames = 0;
}

void Profiler::drawGraph()
{
	std::string fps = "FPS " + std::to_string((int)_frames.getLast());

	ImGui::Begin("FPS Graph");
	ImGui::PlotHistogram(fps.c_str(), _frames.getStart(), FRAME_WINDOW_SIZE, 0, NULL, 0, _frames.getMax() + 20, ImVec2(180, 50));
	ImGui::End();
}
