#include "Core.h"
#include "../Display.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "../Time.h"
#include "Logic.h"
#include "Physics.h"
#include "../PostProcessing.h"
#include "Lighting.h"
#include "../InputManager.h"

#include "../../TestScene.h"
#include "../../PBRScene.h"

bool Core::_isRunning;
//bool Core::_isDebug;

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
	inputManager = new InputManager();


	std::cout << "==================================================" << std::endl;
	std::cout << "               INITIALIZING SYSTEMS" << std::endl << std::endl;

	// Initialize OpenGL
	display->init("PhotonBox Engine", 900, 700);
	renderer->init(Renderer::RenderMode::FORWARD);
	postPocessing->init();
	inputManager->init();

	// Load Scenes
	sceneManager->addScene("TestScene", new TestScene());
	sceneManager->addScene("PBRScene", new PBRScene());
	sceneManager->loadSceneImediately("TestScene");

	// Start Subsystems
	start();

	std::cout << std::endl << "                   SYSTEMS READY" << std::endl;
	std::cout << "==================================================" << std::endl << std::endl;
}

void Core::start() {
	logic->start();
	renderer->start();
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

		postPocessing->preProcess();

		// Render Scene
		renderer->render();
		nbFrames++;
		inputManager->pollEvents();

		postPocessing->postProcess();

		// End of Frame
		if (sceneManager->loadQueuedScene())
			start();
		
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
}
