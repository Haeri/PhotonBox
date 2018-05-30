#ifndef CAMERA_CONTROLLER_SCRIPT_CPP
#define CAMERA_CONTROLLER_SCRIPT_CPP

#include <Components/Behaviour.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <Core/InputManager.h>
#include <Core/Time.h>

class CameraControllerScript : public Behaviour
{
public:
	float speed = 1;
	float mouseSensitivity = 0.6f;

	void Update()
	{
		Vector3f direction = Vector3f::ZERO;
		float shift = 1;
		if (InputManager::keyDown(InputManager::KEY_W)) direction.z() = 1;
		if (InputManager::keyDown(InputManager::KEY_S)) direction.z() = -1;
		if (InputManager::keyDown(InputManager::KEY_D)) direction.x() = -1;
		if (InputManager::keyDown(InputManager::KEY_A)) direction.x() = 1;
		if (InputManager::keyDown(InputManager::KEY_SPACE)) direction.y() = 1;
		if (InputManager::keyDown(InputManager::KEY_LEFT_CONTROL)) direction.y() = -1;

		if (InputManager::keyDown(InputManager::KEY_LEFT_SHIFT)) shift = 2.5f;

		transform->setPosition(transform->getPosition() +
			(transform->forward() * direction.z() +
				transform->up() * direction.y() +
				transform->right() * direction.x())

			* Time::deltaTime * speed * shift);

		Vector2f mouse = InputManager::getMouseDelta();

		float x = transform->getRotation().x() + mouse.y() * Time::deltaTimef * mouseSensitivity;
		float y = transform->getRotation().y() + mouse.x() * Time::deltaTimef * mouseSensitivity;
		float z = transform->getRotation().z();
		transform->setRotation(Vector3f(x, y, z));


		if (InputManager::keyDown(InputManager::KEY_PAGE_UP))
			Camera::getMainCamera()->setFOV(Camera::getMainCamera()->getFOV() + 0.1f);
		if (InputManager::keyDown(InputManager::KEY_PAGE_DOWN))
			Camera::getMainCamera()->setFOV(Camera::getMainCamera()->getFOV() - 0.1f);
	}
};

#endif // CAMERA_CONTROLLER_SCRIPT_CPP