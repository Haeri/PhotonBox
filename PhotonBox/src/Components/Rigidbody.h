#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Core/Component.h"
#include "../Core/Systems/Physics.h"
#include "../Math/Vector3f.h"

class Rigidbody : public Component
{
public:
	Rigidbody()
	{
		//Physics::addPhysicsObject(this); // , this->entity->getComponent<Collider>());
		//Physics::addToPhysicsList(this);
		Physics::registerObject(this);
	}

	~Rigidbody()
	{
		Physics::removePhysicsObject(this);
		//Physics::removeFromPhysicsList(this);
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
