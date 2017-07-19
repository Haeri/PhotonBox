#ifndef CORE_H
#define CORE_H

#include <string>

#include "Display.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Time.h"
#include "Logic.h"
#include "Physics.h"
#include "PostProcessing.h"
#include "Shader.h"
#include "TestScene.cpp"

class Core{
public:
	SceneManager sceneManager;
	Renderer renderer;
	Time theTime;
	Logic logic;
	Physics physics;
	PostProcessing postPocessing;
	Display display;

	Shader shader;
	TestScene* testScene;

	Core();
	void init();
	void update();
	void destroy();
	static Core* getInstance() { return _instance; }
private:
	static Core* _instance;
	double _accumulatedTime;
	static const double FIXED_TIME_INTERVAL;
};

#endif /* defined(PROJECT_MAIN_H) */
