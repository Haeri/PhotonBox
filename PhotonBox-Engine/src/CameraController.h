#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Components/Behaviour.h"
#include "Components/Transform.h"
#include "Core/Time.h"
#include "Core/InputManager.h"

class CameraController : public Behaviour {
public:
	float speed = 3;
	float mouseIntensity = 0.6f;

	bool toggle = true;

	void Start() {
		toggleCursor();
	}

	void Update() {
		Vector3f direction = Vector3f::ZERO;
		float shift = 1;
		if (InputManager::keyPressed(InputManager::KEY_W)) direction.z() = 1;
		if (InputManager::keyPressed(InputManager::KEY_S)) direction.z() = -1;
		if (InputManager::keyPressed(InputManager::KEY_D)) direction.x() = -1;
		if (InputManager::keyPressed(InputManager::KEY_A)) direction.x() = 1;
		if (InputManager::keyPressed(InputManager::KEY_SPACE)) direction.y() = 1;
		if (InputManager::keyPressed(InputManager::KEY_LEFT_CONTROL)) direction.y() = -1;
		
		if (InputManager::keyPressed(InputManager::KEY_LEFT_SHIFT)) shift = 2.5f;

		transform->setPosition(transform->getPosition() +
			(transform->forward() * direction.z() +
			transform->up() * direction.y() +
			transform->right() * direction.x())
			
			* Time::deltaTime * speed * shift);

		Vector2f mouse = InputManager::getMouseDelta();

		float x = transform->getRotation().x() + mouse.y() * Time::deltaTime * mouseIntensity;
		float y = transform->getRotation().y() + mouse.x() * Time::deltaTime * mouseIntensity;
		float z = transform->getRotation().z();
		transform->setRotation(Vector3f(x, y, z));

		if (InputManager::keyPressed(InputManager::KEY_ESCAPE))
			toggleCursor();
	}

	void toggleCursor() {
		if (toggle) {
			InputManager::setCursorMode(InputManager::CursorMode::DISABLED);
		}else {
			InputManager::setCursorMode(InputManager::CursorMode::NORMAL);
		}
		toggle = !toggle;
	}
};

#endif // CAMERA_CONTROLLER_H

