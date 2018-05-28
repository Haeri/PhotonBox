#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Core/Component.h"
#include "../Core/Systems/Physics.h"

class Rigidbody : public Component
{
public:
	Rigidbody()
	{
		//Physics::addPhysicsObject(this); // , this->entity->getComponent<Collider>());
		//Physics::addToPhysicsList(this);
	}

	void delay()
	{
		Physics::addPhysicsObject(this);
	}

	~Rigidbody()
	{
		//Physics::removeFromPhysicsList(this);
	}
};

#endif // RIGIDBODY_H
