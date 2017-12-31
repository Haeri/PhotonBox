#include "Core.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Logic.h"
#include "Physics.h"
#include "Lighting.h"
#include "UIRenderer.h"
#include "../Display.h"
#include "../Time.h"
#include "../Systems/PostProcessing.h"
#include "../InputManager.h"
#include "../../Resources/Config.h"

#include "../../Game/TestScene.h"
#include "../../Game/PBRScene.h"

//#define DEBUG = false;

bool Core::_isRunning;

void Core::init(){
	std::cout << "==================================================" << std::endl;
	std::cout << "               INITIALIZING SYSTEMS" << std::endl << std::endl;

	// Load Config
	config = new Config();
	config->readConfig();

	// Core Systems
	display = new Display();
	inputManager = new InputManager();
	time = new Time();

	// Subsystems
	uiRenderer = new UIRenderer();
	sceneManager = new SceneManager();
	renderer = new Renderer();
	logic = new Logic();
	physics = new Physics();
	postPocessing = new PostProcessing();
	lighting = new Lighting();

	// Initialize OpenGL
	display->init("PhotonBox Engine", Config::profile.width, Config::profile.height, Config::profile.fullscreen, Config::profile.vsync);

	renderer->init(Config::profile.supersampling ? 2 : 1);
	inputManager->init();
	uiRenderer->init();

	std::cout << std::endl << "                   SYSTEMS READY" << std::endl;
	std::cout << "==================================================" << std::endl << std::endl;
	
	// Load Scenes
	sceneManager->addScene("Realistic Rendering", new TestScene());
	sceneManager->addScene("Material Test", new PBRScene());

	sceneManager->loadSceneImediately("Realistic Rendering");

	// Start Subsystems
	start();
}

void Core::start() {
	std::cout << "==================================================" << std::endl;
	std::cout << "                  LOADING SCENE " << SceneManager::getCurrentName() << std::endl << std::endl;
	
	logic->start();
	renderer->start();
	lighting->start();
	postPocessing->start();

	std::cout << std::endl << "                   SCENE READY" << std::endl;
	std::cout << "==================================================" << std::endl << std::endl;
}

void Core::run(){
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	double lastSecond = 0;
	_isRunning = true;

	std::string statPrint;

	while (_isRunning && display->isRunning()) {

		// Measure time
		double currentTime = glfwGetTime();
		time->setDeltaTime(currentTime - lastTime);
		lastTime = currentTime;
		lastSecond += Time::deltaTime;

		if (lastSecond >= 1.0) {
			statPrint = std::to_string(nbFrames) + " FPS  -  " + std::to_string(1000.0f / double(nbFrames)).substr(0, 4) + "ms";
			nbFrames = 0;
			lastSecond = 0;
		}

		// Update Logic
		logic->update();

		// Late update Logic
		logic->lateUpdate();

		// Update Physics
		_accumulatedTime += Time::deltaTime;
		if (_accumulatedTime > FIXED_TIME_INTERVAL) {
			physics->update();
			logic->fixedUpdate();
			_accumulatedTime = 0;
		}

		// Update input
		inputManager->update();


		// Start Rendering
		FrameBuffer::resetDefaultBuffer();
		Display::clearBuffers();

		// Render gBuffers
		renderer->prePass();

		// Render Scene
		Renderer::render();
		nbFrames++;

		postPocessing->postProcess();
		


		// Gizmos
		renderer->renderGizmos();

		// UI Rendering
		uiRenderer->renderText(statPrint, 10, Display::getHeight() - 20, 0.32f, Vector3f(0, 1, 0));
		if (Renderer::isDebug) {
	//		uiRenderer->renderText("Scene: " + sceneManager->getCurrentName(), 10, Display::getHeight() - 35, 0.32f, Vector3f(0.9, 0.9, 0.9));
	//		uiRenderer->renderText("GameObjects:\n" + SceneManager::getCurrentScene()->getGameObjects(), 10, Display::getHeight() - 50, 0.32f, Vector3f(0.9, 0.9, 0.9));
	//		uiRenderer->renderText("Behaviour:\n" + Logic::getList(), 150, Display::getHeight() - 50, 0.32f, Vector3f(0.9, 0.9, 0.9));
		}

		// Stop Rendering
		Display::swapBuffer();


		inputManager->pollEvents();

		// End of Frame
		if (sceneManager->sceneQueued()) {
			sceneManager->unloadScene(SceneManager::getCurrentScene());
			reset();
			sceneManager->loadQueuedScene();
			start();
		}
	}
}

void Core::stop()
{
	_isRunning = false;
}

void Core::reset() {
	postPocessing->reset();
}

void Core::destroy(){
	logic->destroy();
	renderer->destroy();
	physics->destroy();
	sceneManager->destroy();
	display->destroy();

	delete sceneManager;
	delete renderer;
	delete time;
	delete logic;
	delete physics;
	delete postPocessing;
	delete display;
	delete lighting;
	delete config;
}
