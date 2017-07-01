#include "../include/Core.h"

Core::Core(): FIXED_TIME_INTERVAL(0)
{

}

void Core::init()
{
}

void Core::update()
{

    // Update Logic
	Logic::update();
	
	// Update Physics
	accumulatedTime += Time::deltaTime;
	if (accumulatedTime > FIXED_TIME_INTERVAL) {
		Physics::update();
		Logic::fixedUpdate();
		accumulatedTime = 0;
	}

	PostProcessing::preProcess();

	// Render Scene
	renderer.render();

	PostProcessing::postProcess();
	
	// Late update Logic
	Logic::lateUpdate();

}

void Core::destroy()
{
	Logic::destroy();
	renderer.destroy();
	Physics::destroy();
	SceneManager::destroy();
}