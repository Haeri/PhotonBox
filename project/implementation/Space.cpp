#ifndef SPACE_HPP
#define SPACE_HPP

#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "ParticleSystem.cpp"
#include "Logic.h"
#include "AsteroidRenderer.cpp"
#include "ShipRenderer.cpp"
#include "MoveScript.cpp"
#include "Physics.h"
#include "FreeCameraScript.cpp"
#include "RotationScript.cpp"
#include "SphereCollider.cpp"
#include "ShipScript.cpp"
#include "MoveScript.cpp"
#include "CubeRenderer.cpp"
#include "TheRenderer.h"

class Space: public Scene {
public:
	void load() {

		std::cout << "=========================" << std::endl;
		
        GameObject *Camera = instanciate("Camera");
        Camera->addComponent<FreeCameraScript>();

		for (int i = 0; i < 30; ++i) {
			GameObject *Asteroid = instanciate("Asteroid" + std::to_string(i));
			Asteroid->addComponent<AsteroidRenderer>();
			//Asteroid->addComponent<RotationScript>();
			float randScale = (rand() % 200) / 50.0f;
			Asteroid->addComponent<SphereCollider>()->radius = randScale;
			Asteroid->getComponent<Transform>()->scale = vmml::Vector3f(randScale, randScale, randScale);
			Asteroid->getComponent<Transform>()->position = vmml::Vector3f(rand() % 200, rand() % 60, rand() % 200);
		}

        GameObject *Ship = instanciate("Ship");
        Ship->getComponent<Transform>()->position = vmml::Vector3f(0, 0, 0);
		Ship->addComponent<ShipRenderer>();
		Ship->addComponent<SphereCollider>()->radius = 1;
		//Ship->addComponent<ShipScript>();
        
        // Skybox
        GameObject *Cube = instanciate("Cube");
        Cube->addComponent<CubeRenderer>();
        // Put the skybox at the same place as the camera
        Cube->getComponent<Transform>()->position = Camera->getComponent<Transform>()->position;
        // TODO: set the scale at the limit of the rendered scene
        // Scale factor must be big enough so that the disappearance (exit from the box) of other game objects is less visible
        Cube->getComponent<Transform>()->scale = vmml::Vector3f(5000, 5000, 5000);
        
		std::cout << "=========================" << std::endl;

	}
};

#endif /* defined(SPACE_HPP) */
