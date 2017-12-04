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

#include "../../Game/TestScene.h"
#include "../../Game/PBRScene.h"

//#define DEBUG = false;

bool Core::_isRunning;

void Core::init(){
	std::cout << "==================================================" << std::endl;
	std::cout << "               INITIALIZING SYSTEMS" << std::endl << std::endl;

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
	display->init("PhotonBox Engine", 1480, 900);
	renderer->init(1);
	inputManager->init();
	uiRenderer->init();

	std::cout << std::endl << "                   SYSTEMS READY" << std::endl;
	std::cout << "==================================================" << std::endl << std::endl;

	// Load Scenes
	sceneManager->addScene("TestScene", new TestScene());
	sceneManager->addScene("PBRScene", new PBRScene());

	// Start Subsystems
	start();
}

void Core::start() {
	std::cout << "==================================================" << std::endl;
	std::cout << "                  LOADING SCENE" << std::endl << std::endl;
	
	sceneManager->loadSceneImediately("TestScene");
	
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

	while (_isRunning) {
		
		//std::cout << "------------------------------START NEW FRAME------------------------------" << std::endl;
		

		// Measure time
		double currentTime = glfwGetTime();
		time->setDeltaTime(currentTime - lastTime);
		lastTime = currentTime;
		lastSecond += Time::deltaTime;

		if (lastSecond >= 1.0) {
			statPrint = std::to_string(nbFrames) + " FPS  -  " + std::to_string(1000.0f / double(nbFrames)).substr(0, 4) + "ms/Frame";
			//printf("%i FPS - %f ms/frame\n", nbFrames, 1000.0 / double(nbFrames));
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

		// Render Scene
		Renderer::render();
		nbFrames++;

		postPocessing->postProcess();
		

		// Gizmos
		renderer->renderGizmos();


		// UI Rendering
		uiRenderer->renderText(statPrint, 10, Display::getHeight() - 20, 0.32f, Vector3f(0.9, 0.9, 0.9));
		if (Renderer::isDebug) {
	//		uiRenderer->renderText("Scene: " + sceneManager->getCurrentName(), 10, Display::getHeight() - 35, 0.32f, Vector3f(0.9, 0.9, 0.9));
	//		uiRenderer->renderText("GameObjects:\n" + SceneManager::getCurrentScene()->getGameObjects(), 10, Display::getHeight() - 50, 0.32f, Vector3f(0.9, 0.9, 0.9));
	//		uiRenderer->renderText("Behaviour:\n" + Logic::getList(), 150, Display::getHeight() - 50, 0.32f, Vector3f(0.9, 0.9, 0.9));
		}

		// Stop Rendering
		Display::swapBuffer();


		inputManager->pollEvents();

		// End of Frame
		if (sceneManager->loadQueuedScene())
			start();
		
		_isRunning = display->isRunning();
	}
}

void Core::destroy(){
	logic->destroy();
	renderer->destroy();
	physics->destroy();
	sceneManager->destroy();
	display->destroy();
	postPocessing->destroy();

	delete sceneManager;
	delete renderer;
	delete time;
	delete logic;
	delete physics;
	delete postPocessing;
	delete display;
	delete lighting;
}
