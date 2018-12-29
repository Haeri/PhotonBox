#include "PhotonBox/core/systems/Core.h"

#include "PhotonBox/core/PhotonBoxVersion.h"
#include "PhotonBox/core/Display.h"
#include "PhotonBox/core/InputManager.h"
#include "PhotonBox/core/Profiler.h"
#include "PhotonBox/core/Time.h"
#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/core/systems/Logic.h"
#include "PhotonBox/core/systems/Physics.h"
#include "PhotonBox/core/systems/PostProcessing.h"
#include "PhotonBox/core/systems/Renderer.h"
#include "PhotonBox/core/systems/SceneManager.h"
#include "PhotonBox/core/systems/UIRenderer.h"
#include "PhotonBox/core/systems/DebugGUI.h"
#include "PhotonBox/core/GLError.h"
#include "PhotonBox/resources/Config.h"
#include "PhotonBox/util/FileWatch.h"
#include "PhotonBox/resources/Texture.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

const double Core::FIXED_TIME_INTERVAL = 1.0f / 60.0f;

bool Core::_isRunning;

void Core::init(std::map<std::string, Scene*>& sceneMap, std::string firstScene)
{
	std::cout << PHOTON_BOX_VERSION << "\n";
	std::cout << "==================================================" << std::endl;
	std::cout << "               INITIALIZING SYSTEMS" << std::endl << std::endl;

	// Load Config
	_config = new Config();
	_config->readConfig();

	// Core Systems
	_display = new Display();
	_inputManager = new InputManager();
	_time = new Time();

	// Subsystems
	_uiRenderer = new UIRenderer();
	_sceneManager = new SceneManager();
	_debugGUI = new DebugGUI();
	_renderer = new Renderer();
	_logic = new Logic();
	_physics = new Physics();
	_postPocessing = new PostProcessing();
	_lighting = new Lighting();
	_profiler = new Profiler();
	_fileWatch = new FileWatch();

	// Initialize OpenGL
	_display->init("PhotonBox Engine", Config::profile.width, Config::profile.height, Config::profile.fullscreen, Config::profile.vsync);
	
	// Init Subsystems
	_debugGUI->init();
	_renderer->init(Config::profile.supersampling ? 2.0f : 1.0f);
	_inputManager->init();
	_physics->init();
	_uiRenderer->init();
	std::cout << std::endl << "                   SYSTEMS READY" << std::endl;
	std::cout << "==================================================" << std::endl << std::endl;

	// Load Scenes
	_sceneManager->addScenes(sceneMap);
	_sceneManager->loadSceneImediately(firstScene);

	// Start Subsystems
	start();
}

void Core::start()
{
	std::cout << "==================================================" << std::endl;
	std::cout << "            LOADING SCENE " << SceneManager::getCurrentName() << std::endl << std::endl;

	_logic->start();
	_renderer->start();
	_lighting->start();
	_postPocessing->start();
	_physics->start();

	std::cout << std::endl << "                   SCENE READY" << std::endl;
	std::cout << "==================================================" << std::endl << std::endl;
}

void Core::run()
{
	double lastTime = Time::now();
	int nbFrames = 0;
	double lastSecond = 0;
	_isRunning = true;
	static int flop = -1;

	std::string statPrint;

	while (_isRunning && _display->isRunning())
	{
		// Measure time
		double currentTime = Time::now();
		_time->setDeltaTime(currentTime - lastTime);
		lastTime = currentTime;
		lastSecond += Time::deltaTime;

		if (lastSecond >= 1.0)
		{
			statPrint = std::to_string(nbFrames) + " FPS  -  " + std::to_string(1000.0f / double(nbFrames)).substr(0, 4) + "ms";
			Profiler::addFps(nbFrames);
			nbFrames = 0;
			lastSecond = 0;
		}
		
		_inputManager->pollEvents();
		_debugGUI->newFrame();

		// Update Physics
		_accumulatedTime += Time::deltaTime;
		if (_accumulatedTime > FIXED_TIME_INTERVAL)
		{
			_physics->update(_accumulatedTime);
			_logic->fixedUpdate();
			_accumulatedTime = 0;
		}

		// Update Logic
		_logic->update();

		// Late update Logic
		_logic->lateUpdate();

		// Update input
		_inputManager->update();


		// Start Rendering
		FrameBuffer::clearDefaultBuffer();
		_check_gl_error("First Clear", 0);


		// Render gBuffers
		_renderer->prePass();
		_check_gl_error("Pre Pass", 0);


		// Render Scene
		Renderer::renderDeferred();
		nbFrames++;
		_check_gl_error("Main Render", 0);


		_postPocessing->postProcess();
		_check_gl_error("PostProcessing", 0);


		// Gizmos
		_renderer->renderGizmos();
		_check_gl_error("Gizmos", 0);


		FrameBuffer::resetDefaultBuffer();

		// UI Rendering
		if(Config::profile.showFPS)
			_uiRenderer->renderText(statPrint, 10.0f, Display::getHeight() - 20.0f, 0.32f, Vector3f(0, 1, 0));
		if (Config::profile.fpsProfiling)
		{
			_uiRenderer->renderText("min: " + std::to_string(Profiler::getMinFps()) , 10.0f, Display::getHeight() - 35.0f, 0.32f, Vector3f(1, 0, 0));
			_uiRenderer->renderText("max: " + std::to_string(Profiler::getMaxFps()), 10.0f, Display::getHeight() - 50.0f, 0.32f, Vector3f(0, 1, 0));
			_uiRenderer->renderText("avg: " + std::to_string(Profiler::getAvgFps()), 10.0f, Display::getHeight() - 65.0f, 0.32f, Vector3f(0, 0, 1));
		}
		_check_gl_error("SystemUI", 0);


		// System GUI
		_sceneManager->drawSceneList();


		_debugGUI->render();
		_check_gl_error("debugGUI", 0);


		// Refeed position updates to physics system
		_physics->refeed();

		// Stop Rendering
		Display::swapBuffer();
		_renderer->clearDrawCalls();

		// Run filewatch every second
		if(lastSecond == 0)
			_fileWatch->checkValidity();

		_check_gl_error("End of frame", 0);


		
		ManagedResource::lazyLoad((flop == -1));
		
		if (ManagedResource::allReady()) {
			if (flop == -1) {
				std::cout << "Generate Lights!\n";
				_lighting->generate();
			}

			++flop;
		}

		// End of Frame
		if (_sceneManager->sceneQueued())
		{
			_sceneManager->unloadScene(SceneManager::getCurrentScene());
			reset();
			_sceneManager->loadQueuedScene();
			start();

			// reset timing
			_accumulatedTime = 0;
			flop = -1;
			lastTime = Time::now();
		}
	}
}

void Core::stop()
{
	_isRunning = false;
}

void Core::reset()
{
	_postPocessing->reset();
	_profiler->reset();
	_physics->reset();
	_renderer->reset();
}

void Core::destroy()
{
	_sceneManager->destroy();
	_logic->destroy();
	_renderer->destroy();
	_physics->destroy();
	_debugGUI->destroy();
	_display->destroy();
	_postPocessing->destroy();

	Shader::clearAll();

	delete _time;
	delete _display;
	delete _inputManager;
	delete _uiRenderer;
	delete _debugGUI;
	delete _sceneManager;
	delete _renderer;
	delete _logic;
	delete _physics;
	delete _postPocessing;
	delete _lighting;
	delete _profiler;
	delete _config;
	delete _fileWatch;
}