#ifndef STATE_CONTROLLER_SCRIPT_CPP
#define STATE_CONTROLLER_SCRIPT_CPP

#include <component/Behaviour.h>
#include <component/Transform.h>
#include <core/Core.h>
#include <core/InputManager.h>
#include <core/system/Renderer.h>
#include <core/system/SceneManager.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

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