#ifndef ROTATION_SCRIPT_H
#define ROTATION_SCRIPT_H

#include "Behaviour.h"
#include "TheTime.h"

class RotationScript : public Behaviour {
public:
	float speed;
	vmml::Vector3f rotationAxis;

	void Start() {
		speed = (rand() % 20 + 1) / 10.0f;
//		rotationAxis = vmml::Vector3f(rand()%1, rand() % 1, rand() % 1).normalize();
		rotationAxis = vmml::Vector3f(1, 0, 0);
	}
	
	void Update() {
		float rotation = transform->rotation.w();
		transform->rotation = vmml::Vector4f(rotationAxis.x(), rotationAxis.y(), rotationAxis.z(), rotation + TheTime::deltaTime * speed);
	}
};

#endif /* defined(ROTATION_SCRIPT_H) */