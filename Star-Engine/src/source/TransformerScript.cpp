#ifndef TRANSFORMER_SCRIPT_CPP
#define TRANSFORMER_SCRIPT_CPP

#include "../header/Behaviour.h"
#include "../header/Transform.h"
#include "../header/Time.h"

class TransformerScript: public Behaviour {
public:
	void Start() {
	
	}

	void Update() {
		//transform->setPosition(Vector3f(transform->getPosition().x() + 0.01f, 0, 0));
		transform->setRotation(Vector3f(0, transform->getRotation().y() + 0.08f, 0));
	}
};

#endif /* defined(TRANSFORMER_SCRIPT_CPP) */