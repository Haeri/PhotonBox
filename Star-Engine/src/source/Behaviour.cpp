#include "../header/Behaviour.h"
#include "../header/Core.h"

Behaviour::Behaviour(){
	Core::getInstance()->logic.addToUpdateList(this);
}

void Behaviour::destroy(){
	OnDestroy();
	Core::getInstance()->logic.removeFromList(this);
}

void Behaviour::Start() {}
void Behaviour::Update() {}
void Behaviour::FixedUpdate() {}
void Behaviour::LateUpdate() {}
void Behaviour::OnDestroy() {}
