#ifndef STATE_CONTROLLER_SCRIPT_CPP
#define STATE_CONTROLLER_SCRIPT_CPP

#include <Components/Behaviour.h>
#include <Components/Transform.h>
#include <Core/InputManager.h>
#include <Core/Systems/Core.h>
#include <Core/Systems/Renderer.h>
#include <Core/Systems/SceneManager.h>

class StateControllerScript : public Behaviour
{
public:
	bool toggleCursorMode = false;
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

			std::string sr = "rot: " + std::to_string(transform->getRotation().getX()) +
				", " + std::to_string(transform->getRotation().getY()) +
				", " + std::to_string(transform->getRotation().getZ()) +
				", " + std::to_string(transform->getRotation().getW());


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
		Renderer::setDebug((Renderer::getDebugMode() + 1) % Renderer::MAX_DEBUG);
	}
};

#endif // STATE_CONTROLLER_SCRIPT_CPP