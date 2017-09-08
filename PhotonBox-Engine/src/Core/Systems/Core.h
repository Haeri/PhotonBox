#ifndef CORE_H
#define CORE_H

#include <string>

class TestScene;
class Display;
class SceneManager;
class Renderer;
class Time;
class Logic;
class Physics;
class PostProcessing;
class Lighting;
class InputManager;

class Core{
public:
	static bool isRunning() { return _isRunning; }

	void init();
	void update();
	void destroy();
private:
	SceneManager* sceneManager;
	Renderer* renderer;
	Time* time;
	Logic* logic;
	Physics* physics;
	PostProcessing* postPocessing;
	Display* display;
	Lighting* lighting;
	InputManager* inputManager;

	TestScene* testScene;

	const double FIXED_TIME_INTERVAL = 30;
	const double TARGET_FPS = 60;

	double _accumulatedTime, _newTime, _lastTime;
	int _fps = 0;

	static bool _isRunning;
};

#endif // CORE_H
