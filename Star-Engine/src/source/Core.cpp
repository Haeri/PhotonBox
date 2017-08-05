#include "../header/Core.h"
#include <time.h>

#include "TestScene.cpp"

Core* Core::_instance;

Core::Core()
{
	if (_instance == nullptr)
		_instance = this;
}

void Core::init()
{
	// Initialize OpenGL
	display.init("Star Engine", 800, 600);
	
	// Load Scenes
	sceneManager.addScene("TestScene", new TestScene());
	sceneManager.loadScene("TestScene");

	// Initializing Subsystems
	logic.start();
	renderer.init();
	postPocessing.init();	
}

void Core::update()
{

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	double lastSecond = 0;


	while (_running) {

		// Measure time
		double currentTime = glfwGetTime();
		time.setDeltaTime(currentTime - lastTime);
		lastTime = currentTime;
		lastSecond += Time::deltaTime;

		if (lastSecond >= 1.0) {
			printf("%i FPS - %f ms/frame\n", nbFrames, 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastSecond = 0;
		}

		// Update Logic
		logic.update();

		// Update Physics
		_accumulatedTime += Time::deltaTime;
		if (_accumulatedTime > FIXED_TIME_INTERVAL) {
			physics.update();
			logic.fixedUpdate();
			_accumulatedTime = 0;
		}

		PostProcessing::preProcess();

		// Render Scene
		renderer.render();
		nbFrames++;

		PostProcessing::postProcess();

		// Late update Logic
		logic.lateUpdate();

		_running = display.isRunning;
	}
}

void Core::destroy()
{
	logic.destroy();
	renderer.destroy();
	physics.destroy();
	sceneManager.destroy();
	display.~Display();

}