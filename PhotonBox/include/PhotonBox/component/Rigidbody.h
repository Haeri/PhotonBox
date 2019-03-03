#ifndef RIGIDBODY_H
#define RIGIDBODY_H

class PxRigidBody;

#include "PhotonBox/core/Component.h"
#include "PhotonBox/math/Vector3f.h"

class Rigidbody : public Component
{
public:
	Rigidbody();
	~Rigidbody();

	void setBody(physx::PxRigidBody* actor);
	void setDensity(float density);
	float getMass() { return _mass; }
	void addForce(Vector3f direction);
private:
	physx::PxRigidBody* _rigidBody;
	float _mass = 1;
};

#endif // RIGIDBODY_H
