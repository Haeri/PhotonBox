#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "PhotonBox/core/Component.h"
#include "PhotonBox/core/systems/Physics.h"
#include "PhotonBox/math/Vector3f.h"

class Rigidbody : public Component
{
public:
	Rigidbody()
	{
		Physics::registerObject(this);
	}

	~Rigidbody()
	{
		Physics::removePhysicsObject(this);
	}

	void setBody(PxRigidBody* actor);
	void setDensity(float density);
	float getMass() { return _mass; }
	void addForce(Vector3f direction);
private:
	PxRigidBody* _rigidBody;
	float _mass = 1;
};

#endif // RIGIDBODY_H
