#include "../include/Collider.h"
#include "../include/Physics.h"

Collider::Collider() {
	Physics::addToPhysicsList(this);
}

void Collider::destroy(){
	Physics::removeFromPhysicsList(this);
}
