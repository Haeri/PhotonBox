#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "../Components/Behaviour.h"
#include "../Components/Transform.h"
#include "../Core/Time.h"
#include "../Core/InputManager.h"
#include "../Core/Systems/Renderer.h"
#include "../Components/Camera.h"
#include "../Core/Systems/SceneManager.h"

class CameraController : public Behaviour {
public:
	float speed = 1;
	float mouseSensitivity = 0.6f;

	bool toggleCursorMode = true;
	bool toggleRenderMode = true;
	bool toggleVsyncMode = true;

	Material* blur;

	void Start() {
		toggleCursor();
	}

	void Update() {
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

		float x = transform->getRotation().x() + mouse.y() * Time::deltaTime * mouseSensitivity;
		float y = transform->getRotation().y() + mouse.x() * Time::deltaTime * mouseSensitivity;
		float z = transform->getRotation().z();
		transform->setRotation(Vector3f(x, y, z));

		if (InputManager::keyPressed(InputManager::KEY_C))
			toggleCursor();

		if (InputManager::keyPressed(InputManager::KEY_V))
			toggleVSync();

		if (InputManager::keyDown(InputManager::KEY_PAGE_UP))
			Camera::getMainCamera()->setFOV(Camera::getMainCamera()->getFOV() + 0.1f);
		if (InputManager::keyDown(InputManager::KEY_PAGE_DOWN))
			Camera::getMainCamera()->setFOV(Camera::getMainCamera()->getFOV() - 0.1f);


		if (InputManager::keyPressed(InputManager::KEY_1))
			SceneManager::loadScene("Realistic Rendering");
		if (InputManager::keyPressed(InputManager::KEY_2))
			SceneManager::loadScene("Material Test");

		if (InputManager::keyPressed(InputManager::KEY_T))
			toggleDebug();

		if (InputManager::keyPressed(InputManager::KEY_ESCAPE))
			Core::stop();


		if (InputManager::keyPressed(InputManager::KEY_ENTER)) {		
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

	void toggleCursor() {
		if (toggleCursorMode) {
			InputManager::setCursorMode(InputManager::CursorMode::DISABLED);
		}else {
			InputManager::setCursorMode(InputManager::CursorMode::NORMAL);
		}
		toggleCursorMode = !toggleCursorMode;
	}

	void toggleVSync() {
		Display::setVSync(toggleVsyncMode);
		toggleVsyncMode = !toggleVsyncMode;
	}

	void toggleDebug() {
		Renderer::setDebug(!Renderer::isDebug());
	}
};

#endif // CAMERA_CONTROLLER_H