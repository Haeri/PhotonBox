#ifndef CORE_H
#define CORE_H

#include "../include/SceneManager.h"
#include "../include/Renderer.h"
#include "../include/Time.h"
#include "../include/Logic.h"
#include "../include/Physics.h"
#include "../include/PostProcessing.h"

class Core{
public:
	SceneManager sceneManager;
	Renderer renderer;
	Time theTime;
	Logic logic;
	Physics physics;
	PostProcessing postPocessing;

	Core();
	void init();
	void update();
	void destroy();
private:
	double accumulatedTime;
	double const FIXED_TIME_INTERVAL;
};

#endif /* defined(PROJECT_MAIN_H) */
