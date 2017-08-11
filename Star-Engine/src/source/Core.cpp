#include "../header/Core.h"
#include "../header/Display.h"
#include "../header/SceneManager.h"
#include "../header/Renderer.h"
#include "../header/Time.h"
#include "../header/Logic.h"
#include "../header/Physics.h"
#include "../header/PostProcessing.h"
#include "../header/Lighting.h"

#include "TestScene.cpp"

bool Core::_isRunning;

void Core::init()
{
	sceneManager = new SceneManager();
	renderer = new Renderer();
	time = new Time();
	logic = new Logic();
	physics = new Physics();
	postPocessing = new PostProcessing();
	display = new Display();
	lighting = new Lighting();
	testScene = new TestScene();

	// Initialize OpenGL
	display->init("Star Engine", 800, 600);
	
	// Load Scenes
	sceneManager->addScene("TestScene", new TestScene());
	sceneManager->loadScene("TestScene");

	// Initializing Subsystems
	logic->start();
	renderer->init();
	postPocessing->init();	
}

void Core::update()
{
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	double lastSecond = 0;
	_isRunning = true;

	while (_isRunning) {

		// Measure time
		double currentTime = glfwGetTime();
		time->setDeltaTime(currentTime - lastTime);
		lastTime = currentTime;
		lastSecond += Time::deltaTime;

		if (lastSecond >= 1.0) {
			printf("%i FPS - %f ms/frame\n", nbFrames, 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastSecond = 0;
		}

		// Update Logic
		logic->update();

		// Update Physics
		_accumulatedTime += Time::deltaTime;
		if (_accumulatedTime > FIXED_TIME_INTERVAL) {
			physics->update();
			logic->fixedUpdate();
			_accumulatedTime = 0;
		}

		PostProcessing::preProcess();

		// Render Scene
		renderer->render();
		nbFrames++;

		PostProcessing::postProcess();

		// Late update Logic
		logic->lateUpdate();

		_isRunning = display->isRunning();
	}
}

void Core::destroy()
{
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
	delete testScene;
}