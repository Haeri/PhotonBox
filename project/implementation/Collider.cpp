#include "Collider.h"
#include "Physics.h"

Collider::Collider() {
	Physics::addToPhysicsList(this);
}
