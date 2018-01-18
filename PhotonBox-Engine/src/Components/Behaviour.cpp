#include "Behaviour.h"
#include "../Core/Systems/Logic.h"

Behaviour::Behaviour()
{
	Logic::addToUpdateList(this);
}

void Behaviour::destroy()
{
	OnDestroy();
	Logic::removeFromList(this);
}

void Behaviour::Start() {}
void Behaviour::Update() {}
void Behaviour::FixedUpdate() {}
void Behaviour::LateUpdate() {}
void Behaviour::OnDestroy() {}
