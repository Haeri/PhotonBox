#include "Component.h"

void Component::setEntity(Entity* _go)
{
	entity = _go;
}

void Component::setTransform(Transform* _t)
{
	transform = _t;
}

void Component::setEnable(bool enable)
{
	if (enable)
		OnEnable();
	else
		OnDisable();
	isEnabled = enable;
}

std::string Component::getName()
{
	std::string s = typeid(*this).name();
	return s.substr(6);
}

void Component::OnEnable() {}
void Component::OnDisable() {}
void Component::destroy() {}

std::type_index Component::getBaseType()
{
	return typeid(Component);
}

Component::~Component() {}
