#include "../header/Collider.h"
#include "../header/Core.h"

Collider::Collider() {
	Core::getInstance()->physics.addToPhysicsList(this);
}

void Collider::destroy(){
	Core::getInstance()->physics.removeFromPhysicsList(this);
}