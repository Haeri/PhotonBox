#include "Transform.h"
#include "GameObject.h"

void Transform::setParent(Transform *_parent) {
	parent = _parent;
}

void Transform::setParent(GameObject *_gameObject) {
	parent = _gameObject->transform;
}

vmml::Matrix4f Transform::getTransformationMatrix()
{
	if (parent != nullptr) {
		return (parent->getTransformationMatrix()) * getLocalTransformationMatrix();
	}
	else
		return getLocalTransformationMatrix();
}

vmml::Matrix4f Transform::getLocalTransformationMatrix()
{
	return  vmml::create_translation(position) * vmml::create_rotation(rotation.w(), vmml::Vector3f(rotation.x(), rotation.y(), rotation.z())) * vmml::create_scaling(scale) * vmml::Matrix4f::IDENTITY;
}
