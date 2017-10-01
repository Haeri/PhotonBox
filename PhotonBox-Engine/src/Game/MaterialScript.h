#ifndef MATERIAL_SCRIPT_H
#define MATERIAL_SCRIPT_H

#include "../Components/Behaviour.h"
#include "../Components/Transform.h"
#include "../Core/Time.h"
#include "../Core/InputManager.h"
#include "../Core/Systems/Renderer.h"
#include "../Components/Camera.h"
#include "../Core/Systems/SceneManager.h"
#include "../Resources/Material.h"

class MaterialScript : public Behaviour {
public:
	Material* material;

	void Update() {
		if (InputManager::keyPressed(InputManager::KEY_UP))
			material->getUniform<float>("shininess");

		if (InputManager::keyPressed(InputManager::KEY_DOWN))
			

	}
};

#endif // CAMERA_CONTROLLER_H

