#ifndef CORE_H
#define CORE_H

#include <string>

class Display;
class SceneManager;
class Renderer;
class UIRenderer;
class Time;
class Logic;
class Physics;
class PostProcessing;
class Lighting;
class InputManager;
class Config;

class Core{
public:
	static bool isRunning() { return _isRunning; }

	void init();
	void start();
	void reset();
	void run();
	void destroy();
	static void stop();
private:
	Time* time;
	Display* display;
	InputManager* inputManager;
	
	UIRenderer* uiRenderer;
	SceneManager* sceneManager;
	Renderer* renderer;
	Logic* logic;
	Physics* physics;
	PostProcessing* postPocessing;
	Lighting* lighting;

	Config* config;

	const double FIXED_TIME_INTERVAL = 30;
	const double TARGET_FPS = 60;

	double _accumulatedTime, _newTime, _lastTime;
	int _fps = 0;

	static bool _isRunning;
};

#endif // CORE_H
