#include "../include/Component.h"

void Component::setGameObject(GameObject* _go) {
	gameObject = _go;
}

void Component::setTransform(Transform* _t) {
	transform = _t;
}

std::string Component::getName() {
	std::string s = typeid(*this).name();
	return s.substr(6);
}

void Component::setEnable(bool enable) {
	if (enable)
		OnEnable();
	else
		OnDisable();
	isEnabled = enable;
}
void Component::OnEnable() {}
void Component::OnDisable() {}
void Component::destroy() {}

Component::~Component() {}
