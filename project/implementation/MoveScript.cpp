#ifndef MOVE_SCRIPT_H
#define MOVE_SCRIPT_H

#include "Behaviour.h"
#include "TheTime.h"

class MoveScript : public Behaviour{
public: 
	float speed = 1.0f;

	void Update() {
		vmml::Vector3f pos = transform->position;
		transform->position = pos + vmml::Vector3f(1, 0, 0) * TheTime::deltaTime * speed;
	}
};

#endif /* defined(MOVE_SCRIPT_H) */