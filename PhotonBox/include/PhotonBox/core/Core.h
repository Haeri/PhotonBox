#ifndef CORE_H
#define CORE_H

//#define RECORD_MODE

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
class Profiler;
class Scene;
class DebugGUI;
class FileWatch;
class ISystem;
class Logger;
class ResourceManager;

#include <string>
#include <map>
#include <vector>

class Core
{
public:
	static const double FIXED_TIME_INTERVAL;

	void init(std::map<std::string, Scene*>& sceneMap);
	void start();
	void reset();
	void run();
	void destroy();

	static bool isRunning() { return _isRunning; }
	static void stop();
private:
	Time*			_time;
	Display*		_display;
	InputManager*	_inputManager;
	UIRenderer*		_uiRenderer;
	DebugGUI*		_debugGUI;
	SceneManager*	_sceneManager;
	Renderer*		_renderer;
	Logic*			_logic;
	Physics*		_physics;
	PostProcessing* _postPocessing;
	Lighting*		_lighting;

	Profiler*		_profiler;
	Config*			_config;
	FileWatch*		_fileWatch;
	Logger*			_logger;
	ResourceManager* _resourceManager;

	std::vector<ISystem*> _systems;

	double _accumulatedTime, _newTime, _lastTime;

	static bool _isRunning;
	unsigned char* _record_data;
};
#endif // CORE_H
