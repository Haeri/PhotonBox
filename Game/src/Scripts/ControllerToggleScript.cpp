#ifndef CONTROLLER_TOGGLE_SCRIPT_CPP
#define CONTROLLER_TOGGLE_SCRIPT_CPP

#include <Components/Behaviour.h>

class ControllerToggleScript : public Behaviour
{
public:
	CameraControllerScript * cameraController;
	PathWalkerScript* pathWalkerScript;


	void Start()
	{
		cameraController = gameObject->getComponent<CameraControllerScript>();
		pathWalkerScript = gameObject->getComponent<PathWalkerScript>();

		cameraController->setEnable(false);
		pathWalkerScript->setEnable(true);
	}

	void Update()
	{
		if (InputManager::keyPressed(InputManager::KEY_BACKSPACE))
			switchState();
	}

	void switchState()
	{
		cameraController->setEnable(!cameraController->getEnable());
		pathWalkerScript->setEnable(!pathWalkerScript->getEnable());
	}
};

#endif // CONTROLLER_TOGGLE_SCRIPT_CPP