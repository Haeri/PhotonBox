#include "Behaviour.h"

Behaviour::Behaviour() {
	Logic::addToUpdateList(this);
}

void Behaviour::Start() {}
void Behaviour::Update() {}
void Behaviour::FixedUpdate() {}
void Behaviour::LateUpdate() {}
void Behaviour::Destroy() {}