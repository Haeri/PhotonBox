#ifndef FOLLOW_SCRIPT_H
#define FOLLOW_SCRIPT_H

#include "../Components/Behaviour.h"
#include "../Components/Transform.h"
#include "../Core/InputManager.h"
#include "../Resources/Material.h"

class FollowScript : public Behaviour {
public:
	Transform* target;

	void Update() {
		target->setPosition(transform->getPosition());
	}
};

#endif // FOLLOW_SCRIPT_H

