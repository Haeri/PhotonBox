#include "../header/Core.h"
#include <time.h>
#include <chrono>
#include <thread>

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

	// Timing
	long lastFpsTime = 0;
	int frameCount = 0;
	long lastLoopTime = Time::now();
	double OPTIMAL_TIME = 1000000000.0 / TARGET_FPS;


	while (display.isRunning) {

		long now = Time::now();
		time.setTime(now);
		long updateLength = now - lastLoopTime;
		lastLoopTime = now;
		double delta = updateLength / ((double)OPTIMAL_TIME);
		time.setDeltaTime(delta);
	
		

		//std::cout << "DeltaTime: " << Time::deltaTime << std::endl;

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

		PostProcessing::postProcess();

		// Late update Logic
		logic.lateUpdate();



		// update the frame counter
		lastFpsTime += updateLength;
		frameCount++;

		// update FPS
		if (lastFpsTime >= 1000000000) {
			FPS = frameCount;
			lastFpsTime = 0;
			frameCount = 0;

			std::cout << "FPS: " << FPS << std::endl;
		}

		// wait for present
		if (TARGET_FPS != -1) {
			long wait = ((lastLoopTime - Time::now() + OPTIMAL_TIME) / 1000000);
			std::this_thread::sleep_for(std::chrono::milliseconds(wait));
		}
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