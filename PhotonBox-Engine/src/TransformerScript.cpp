#ifndef TRANSFORMER_SCRIPT_CPP
#define TRANSFORMER_SCRIPT_CPP

#include "Components/Behaviour.h"
#include "Components/Transform.h"
#include "Core/Time.h"

class TransformerScript: public Behaviour {
public:
	float speed = -0.5f;

	void Start() {
	
	}

	void Update() {
		//transform->setPosition(Vector3f(transform->getPosition().x() + 0.01f, 0, 0));
		transform->setRotation(Vector3f(0, transform->getRotation().y() + Time::deltaTime * speed, 0));
	}
};

#endif /* defined(TRANSFORMER_SCRIPT_CPP) */
