#include "../header/Core.h"

Core* Core::_instance;
const double Core::FIXED_TIME_INTERVAL = 0;


Core::Core()
{
	if (_instance == nullptr)
		_instance = this;
}

void Core::init()
{
	display.init("Star Engine", 800, 600);

	sceneManager.addScene("TestScene", new TestScene());
	sceneManager.loadScene("TestScene");

	logic.start();
	renderer.init();
	postPocessing.init();
}

void Core::update()
{
	while (display.isRunning) {

		// Update Logic
		logic.update();

		// Update Physics
		_accumulatedTime += Time::deltaTime;
		if (_accumulatedTime > FIXED_TIME_INTERVAL) {
			Physics::update();
			logic.fixedUpdate();
			_accumulatedTime = 0;
		}

		PostProcessing::preProcess();

		// Render Scene
		renderer.render();

		PostProcessing::postProcess();

		// Late update Logic
		logic.lateUpdate();
	}

}

void Core::destroy()
{
	logic.destroy();
	renderer.destroy();
	Physics::destroy();
	sceneManager.destroy();
	display.~Display();

}