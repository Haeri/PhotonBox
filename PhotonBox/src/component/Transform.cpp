#include "PhotonBox/component/Transform.h"

#include <algorithm>

#include "PhotonBox/component/Camera.h"
#include "PhotonBox/core/Entity.h"
#include "PhotonBox/core/system/DebugGUI.h"
#include "PhotonBox/math/Math.h"
#include "PhotonBox/core/system/Logic.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Vector3f Transform::forward()
{
	Matrix4f r = getRotationMatrix();
	return Vector3f(r(0, 2), r(1, 2), r(2, 2));
}

Vector3f Transform::forwardWorld()
{
	Matrix4f r = getTransformationMatrix();
	return Vector3f(r(0, 2), r(1, 2), r(2, 2));
}

Vector3f Transform::up()
{
	Matrix4f r = getRotationMatrix();
	return Vector3f(r(0, 1), r(1, 1), r(2, 1));
}

Vector3f Transform::upWorld()
{
	Matrix4f r = getTransformationMatrix();
	return Vector3f(r(0, 1), r(1, 1), r(2, 1));
}

Vector3f Transform::right()
{
	Matrix4f r = getRotationMatrix();
	return Vector3f(r(0, 0), r(1, 0), r(2, 0));
}

Vector3f Transform::rightWorld()
{
	Matrix4f r = getTransformationMatrix();
	return Vector3f(r(0, 0), r(1, 0), r(2, 0));
}

void Transform::setChanged()
{
	_lastChangedTick = Logic::getTickIndex();
	_isMatrixCached = false;
}

bool Transform::hasChangedSince(unsigned long int tick)
{
	return tick != _lastChangedTick;
}

bool Transform::hasChangedLastTick()
{
	return Logic::getTickIndex() == _lastChangedTick;
}

Transform * Transform::getParent()
{
	return _parent;
}

std::vector<Transform*> Transform::getChildren()
{
	return _children;
}

void Transform::removeChild(Transform * child)
{
	_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
}

void Transform::renderHandels()
{
	{
		ImGui::BeginGroup();
		ImGui::Text(entity->name.c_str());
		ImGui::SliderFloat3(("Pos##" + entity->name).c_str(), &(_position[0]), -10, 10);
		//ImGui::SliderFloat4("Rotation", &(_rotation[0]), -10, 10);
		setChanged();
		ImGui::EndGroup();
		ImGui::NewLine();
	}
}

Matrix4f Transform::getRotationMatrix()
{
	return _rotation.createRotation();
}

Quaternion Transform::getRotation()
{
	return _rotation;
}

Vector3f Transform::getPosition()
{
	return _position;
}

Vector3f Transform::getPositionWorld()
{
	Matrix4f temp = getTransformationMatrix();
	return Vector3f(temp(3, 0), temp(3, 1), temp(3, 2));
}

Vector3f Transform::getScale()
{
	return _scale;
}

void Transform::setPosition(Vector3f position)
{
	if (!position.equals(_position))
	{
		_position = position;
		setChanged();
	}
}

void Transform::setRotation(Vector3f rotation)
{
	setRotation(Quaternion(rotation));
}

void Transform::setRotation(Quaternion rotation)
{
	if (!rotation.equals(_rotation))
	{
		_rotation = rotation;
		setChanged();
	}
}

void Transform::setScale(Vector3f scale)
{
	if (!scale.equals(_scale))
	{
		_scale = scale;
		setChanged();
	}
}

void Transform::setParent(Transform *parent)
{
	if (_parent == parent) return;

	if (_parent != nullptr)
	{
		_parent->removeChild(this);
	}

	_parent = parent;

	if (parent != nullptr)
		parent->_children.push_back(this);

	setChanged();
}

void Transform::setParent(Entity *_entity)
{
	setParent(_entity->transform);
}

void Transform::rotate(Quaternion quat)
{
	_rotation = (quat * _rotation).normalize();
	setChanged();
}

Matrix4f Transform::getTransformationMatrix()
{
	if (_parent != nullptr)
		return (_parent->getTransformationMatrix()) * getLocalTransformationMatrix();
	else
		return getLocalTransformationMatrix();
}

Matrix4f Transform::getLocalTransformationMatrix()
{
	if (!_isMatrixCached)
	{
		_transformationMatrixCached = Matrix4f::IDENTITY;
		_transformationMatrixCached = getRotationMatrix() * Matrix4f::createScaling(_scale);

		_transformationMatrixCached(3, 0) = _position.x();
		_transformationMatrixCached(3, 1) = _position.y();
		_transformationMatrixCached(3, 2) = _position.z();

		_isMatrixCached = true;
	}

	return _transformationMatrixCached;
}
