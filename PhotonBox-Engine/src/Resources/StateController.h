#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include "../Components/Behaviour.h"
#include "../Components/Transform.h"
#include "../Core/InputManager.h"
#include "../Core/Systems/Core.h"
#include "../Core/Systems/Renderer.h"
#include "../Core/Systems/SceneManager.h"

class StateController : public Behaviour
{
public:
	bool toggleCursorMode = true;
	bool toggleRenderMode = true;
	bool toggleVsyncMode = true;

	void Start()
	{
		toggleCursor();
	}

	void Update()
	{
		if (InputManager::keyPressed(InputManager::KEY_C))
			toggleCursor();

		if (InputManager::keyPressed(InputManager::KEY_V))
			toggleVSync();


		if (InputManager::keyPressed(InputManager::KEY_1))
			SceneManager::loadScene("Realistic Rendering");
		if (InputManager::keyPressed(InputManager::KEY_2))
			SceneManager::loadScene("Material Test");

		if (InputManager::keyPressed(InputManager::KEY_B))
			toggleDebug();

		if (InputManager::keyPressed(InputManager::KEY_ESCAPE))
			Core::stop();

		if (InputManager::keyPressed(InputManager::KEY_ENTER))
		{
			std::string sp = "pos: " + std::to_string(transform->getPositionWorld().x()) +
				", " + std::to_string(transform->getPositionWorld().y()) +
				", " + std::to_string(transform->getPositionWorld().z());

			std::string sr = "rot: " + std::to_string(transform->getRotation().x()) +
				", " + std::to_string(transform->getRotation().y()) +
				", " + std::to_string(transform->getRotation().z());


			std::cout << sp << std::endl;
			std::cout << sr << std::endl << std::endl;
		}
	}

	void toggleCursor()
	{
		if (toggleCursorMode)
		{
			InputManager::setCursorMode(InputManager::CursorMode::DISABLED);
		}
		else
		{
			InputManager::setCursorMode(InputManager::CursorMode::NORMAL);
		}
		toggleCursorMode = !toggleCursorMode;
	}

	void toggleVSync()
	{
		Display::setVSync(toggleVsyncMode);
		toggleVsyncMode = !toggleVsyncMode;
	}

	void toggleDebug()
	{
		Renderer::setDebug(!Renderer::isDebug());
	}
};

#endif // STATE_CONTROLLER_H
