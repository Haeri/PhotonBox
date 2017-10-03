#ifndef MATERIAL_SCRIPT_H
#define MATERIAL_SCRIPT_H

#include "../Components/Behaviour.h"
#include "../Components/Transform.h"
#include "../Core/InputManager.h"
#include "../Resources/Material.h"

class MaterialScript : public Behaviour {
public:
	Material* material;

	void Update() {
		if (InputManager::keyPressed(InputManager::KEY_UP)) {
			float s = material->getProperty<float>("shininess");
			material->setProperty<float>("shininess", s += 100);
		}

		if (InputManager::keyPressed(InputManager::KEY_DOWN)) {
			float s = material->getProperty<float>("shininess");
			material->setProperty<float>("shininess", s -= 100);
		}
		
		//std::cout << material->getProperty<float>("shininess") << std::endl;
	}
};

#endif // CAMERA_CONTROLLER_H

