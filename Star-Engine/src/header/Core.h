#ifndef CORE_H
#define CORE_H

#include <string>

class TestScene;
#include "Display.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Time.h"
#include "Logic.h"
#include "Physics.h"
#include "PostProcessing.h"
#include "Lighting.h"

class Core{
public:
	SceneManager sceneManager;
	Renderer renderer;
	Time time;
	Logic logic;
	Physics physics;
	PostProcessing postPocessing;
	Display display;
	Lighting lighting;

	TestScene* testScene;

	Core();
	void init();
	void update();
	void destroy();

	bool isRunning() { return _running; }
	static Core* getInstance() { return _instance; }
private:
	bool _running = true;
	static Core* _instance;
	double _accumulatedTime, _newTime, _lastTime;


	const double FIXED_TIME_INTERVAL = 30;
	const double TARGET_FPS = 60;
	int FPS = 0;
};

#endif /* defined(PROJECT_MAIN_H) */
