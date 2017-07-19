#include "../header/Collider.h"
#include "../header/Physics.h"

Collider::Collider() {
	Physics::addToPhysicsList(this);
}

void Collider::destroy(){
	Physics::removeFromPhysicsList(this);
}
