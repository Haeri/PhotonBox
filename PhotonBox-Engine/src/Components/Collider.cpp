#include "Collider.h"
#include "../Core/Systems/Physics.h"

Collider::Collider()
{
	Physics::addToPhysicsList(this);
}

void Collider::destroy()
{
	Physics::removeFromPhysicsList(this);
}
