#ifndef MATERIAL_SCRIPT_H
#define MATERIAL_SCRIPT_H

#include "../Components/Behaviour.h"
#include "../Components/Transform.h"
#include "../Core/InputManager.h"
#include "../Resources/Material.h"

class MaterialScript : public Behaviour {
public:
	Material* material;
	//Shader* shader;

	void Start() {
		//shader = SkyBoxShader::getInstance();
	}

	//float mip = 0.1f;

	void Update() {
		if (InputManager::keyDown(InputManager::KEY_UP)) {
			float s = material->getProperty<float>("exposure");
			material->setProperty<float>("exposure", s += 0.1f);
			//std::cout << mip << std::endl;
			//shader->bind();
			//shader->setUniform<float>("mip", mip += 0.1);
		}

		if (InputManager::keyDown(InputManager::KEY_DOWN)) {
			float s = material->getProperty<float>("exposure");
			material->setProperty<float>("exposure", s -= 0.1f);
			
			//float s = material->getProperty<float>("mip");
			//shader->bind();
			//shader->setUniform<float>("mip", mip -= 0.1);
		}
		
		//std::cout << material->getProperty<float>("shininess") << std::endl;
	}
};

#endif // CAMERA_CONTROLLER_H

