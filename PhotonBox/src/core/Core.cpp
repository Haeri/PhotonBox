#include "PhotonBox/core/Core.h"

#include "PhotonBox/core/PhotonBoxVersion.h"
#include "PhotonBox/core/Config.h"
#include "PhotonBox/core/Display.h"
#include "PhotonBox/core/InputManager.h"
#include "PhotonBox/core/Profiler.h"
#include "PhotonBox/core/Time.h"
#include "PhotonBox/core/ISystem.h"
#include "PhotonBox/core/system/Lighting.h"
#include "PhotonBox/core/system/Logic.h"
#include "PhotonBox/core/system/Physics.h"
#include "PhotonBox/core/system/PostProcessing.h"
#include "PhotonBox/core/system/Renderer.h"
#include "PhotonBox/core/system/SceneManager.h"
#include "PhotonBox/core/system/UIRenderer.h"
#include "PhotonBox/core/system/DebugGUI.h"
#include "PhotonBox/core/system/ResourceManager.h"
#include "PhotonBox/resource/Texture.h"
#include "PhotonBox/resource/FrameBuffer.h"
#include "PhotonBox/util/GLError.h"
#include "PhotonBox/util/FileWatch.h"
#include "PhotonBox/util/Logger.h"

#ifdef RECORD_MODE
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "STB/stb_image_write.h"
#endif
#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

const double Core::FIXED_TIME_INTERVAL = 1.0f / 60.0f;

bool Core::_isRunning;

void Core::init(std::map<std::string, Scene*>& sceneMap)
{
	std::cout << PHOTON_BOX_VERSION << "\n";
	std::cout << "==================================================" << std::endl;
	std::cout << "               INITIALIZING SYSTEMS" << std::endl << std::endl;

	_logger = new Logger();

	// Load Config
	_config = new Config();
	_config->readConfig();

	// Core Systems
	_display = new Display();
	_debugGUI = new DebugGUI();
	_renderer = new Renderer();
	_inputManager = new InputManager();
	_physics = new Physics();
	_uiRenderer = new UIRenderer();
	_logic = new Logic();
	_lighting = new Lighting();
	_postPocessing = new PostProcessing();
	_sceneManager = new SceneManager();
	_sceneManager->setSceneMap(sceneMap);

	// Helper
	_profiler = new Profiler();
	_fileWatch = new FileWatch();
	_time = new Time();
	_resourceLoader = new ResourceManager();

	_systems.push_back(_display);
	_systems.push_back(_sceneManager);
	_systems.push_back(_debugGUI);
	_systems.push_back(_renderer);
	_systems.push_back(_inputManager);
	_systems.push_back(_physics);
	_systems.push_back(_uiRenderer);
	_systems.push_back(_logic);
	_systems.push_back(_lighting);
	_systems.push_back(_postPocessing);

	for (std::vector<ISystem*>::iterator it = _systems.begin(); it != _systems.end(); ++it)
	{
		(*it)->init(_config->profile);
	}

#ifdef RECORD_MODE
	const int size = Display::getWidth() * Display::getHeight() * 3;
	_record_data = new unsigned char[size];
#endif

	std::cout << std::endl << "                   SYSTEMS READY" << std::endl;
	std::cout << "==================================================" << std::endl << std::endl;

	// Start Subsystems
	start();
}

void Core::start()
{
	std::cout << "==================================================" << std::endl;
	std::cout << "            LOADING SCENE " << SceneManager::getCurrentName() << std::endl << std::endl;

	for (std::vector<ISystem*>::iterator it = _systems.begin(); it != _systems.end(); ++it)
	{
		(*it)->start();
	}

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
#ifdef RECORD_MODE
		_time->setDeltaTime(1.0 / 24.0);
#else
		_time->setDeltaTime(currentTime - lastTime);
#endif
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

		// Update Physics
		_accumulatedTime += Time::deltaTime;
		if (_accumulatedTime > FIXED_TIME_INTERVAL)
		{
			_physics->refeed();
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
		_check_gl_error("Pre Frame", 0);
		FrameBuffer::clearDefaultBuffer();
		_check_gl_error("First Clear", 0);


		// Fill gBuffers
		_renderer->prePass();
		_check_gl_error("Pre Pass", 0);


		// Render Scene
		Renderer::renderDeferred();
		nbFrames++;
		_check_gl_error("Main Render", 0);

		// Post process
		_postPocessing->postProcess();
		_check_gl_error("PostProcessing", 0);

#ifndef RECORD_MODE
		// Gizmos
		_renderer->renderGizmos();
		_postPocessing->drawGizmos();
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
		_profiler->drawGraph();
		_check_gl_error("SystemUI", 0);


		// System GUI
		_sceneManager->drawSceneList();
#endif
		_debugGUI->render();
		_debugGUI->newFrame();
		_check_gl_error("debugGUI", 0);

		// Refeed position updates to physics system
		//_physics->refeed();

		// Stop Rendering
		Display::swapBuffer();
		_renderer->clearDrawCalls();



		/*
		*	After Render events
		*/

		// Run filewatch every second
		if(lastSecond == 0)
			_fileWatch->checkValidity();

		_check_gl_error("End of frame", 0);


		// Initialize loaded resources
		//_resourceLoader->load((flop != -1));
		_resourceLoader->load();

		if (ResourceManager::isCompleted()) {
			if (flop == -1) {
				std::cout << "Generate Lighting!\n";
				_lighting->generate();
				_renderer->_shadowsAreDirty = true;
			}

			++flop;
		}

		_logic->tick();

#ifdef RECORD_MODE
		int digits = 5;
		std::string s_num = std::to_string(Renderer::getFrameIndex());
		std::string num = std::string(digits-s_num.length(), '0').append(s_num);
		std::string s = "frames/frame_" + num + ".png";
		const char* frame = s.c_str();
		std::cout << "Start " << frame;

		FrameBuffer::resetDefaultBuffer();
		glReadPixels(0, 0, Display::getWidth(), Display::getHeight(), GL_RGB, GL_UNSIGNED_BYTE, _record_data);
		stbi_flip_vertically_on_write(1);
		stbi_write_png(frame, Display::getWidth(), Display::getHeight(), 3, (void*)_record_data, 0);

		std::cout << "- Done\n"; 
#endif

		// End of Frame
		if (_sceneManager->sceneQueued())
		{
			_sceneManager->unloadScene(SceneManager::getCurrentScene());
			reset();
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
	for (std::vector<ISystem*>::iterator  it = _systems.begin(); it != _systems.end(); ++it)
	{
		(*it)->reset();
	}

	_profiler->reset();
	_resourceLoader->reset();
}

void Core::destroy()
{
	FrameBuffer::destroy();
	Shader::clearAll();

	for (std::vector<ISystem*>::reverse_iterator it = _systems.rbegin(); it != _systems.rend(); ++it) {
		(*it)->destroy();
	}

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
	delete _logger;
#ifdef RECORD_MODE
	delete _record_data;
#endif
}