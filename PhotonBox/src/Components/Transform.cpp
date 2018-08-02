#include "PhotonBox/components/Transform.h"

#include <algorithm>

#include "PhotonBox/components/Camera.h"
#include "PhotonBox/core/Entity.h"
#include "PhotonBox/core/systems/DebugGUI.h"
#include "PhotonBox/math/Math.h"

#include "imgui/imgui.h"

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

bool Transform::hasChanged()
{

	//TODO: This is completely broken! This should not be calld since last frame, but needs to be
	// an internal falg for cache checking.
	// Create a public hasChanged variable that is updated every frame
	return true;
}

void Transform::removeChild(Transform * child)
{
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void Transform::renderHandels()
{
	Matrix4f projectionMatrix = Camera::getMainCamera()->getProjectionMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*)&projectionMatrix(0, 0));
	glMatrixMode(GL_MODELVIEW);
	Matrix4f MV = Camera::getMainCamera()->getViewMatrix() * getTransformationMatrix(true, false, true);
	glLoadMatrixf((const GLfloat*)&MV(0, 0));

	Vector4f center = Vector4f::ZERO;
	Vector4f x = center + Vector4f::UNIT_X;
	Vector4f z = center + Vector4f::UNIT_Z;
	Vector4f y = center + Vector4f::UNIT_Y;

	glDepthFunc(GL_ALWAYS);

	glUseProgram(0);

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3fv(&center.x());
	glVertex3fv(&x.x());

	glColor3f(0, 1, 0);
	glVertex3fv(&center.x());
	glVertex3fv(&z.x());

	glColor3f(1, 0, 0);
	glVertex3fv(&center.x());
	glVertex3fv(&y.x());
	glEnd();
	glFinish();

	glDepthFunc(GL_LESS);


	{
		ImGui::BeginGroup();
		ImGui::Text(entity->name.c_str());
		ImGui::SliderFloat3(("Pos##" + entity->name).c_str(), &(_position[0]), -10, 10);
		//ImGui::SliderFloat4("Rotation", &(_rotation[0]), -10, 10);
		ImGui::EndGroup();
		ImGui::NewLine();
	}
}

void Transform::print()
{
	std::cout << entity->name << std::endl;
	std::cout << "Position: " << getPosition() << std::endl;
	std::cout << "World Position: " << getPositionWorld() << std::endl;
	std::cout << "Rotation: " << getRotation() << std::endl;
	std::cout << "Scale: " << getScale() << std::endl;
	std::cout << "Transform: " << std::endl << getTransformationMatrix() << std::endl << std::endl;
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
		_hasChangedLastFrame = true;
	}
}

void Transform::setRotation(Vector3f rotation)
{
	setRotation(Quaternion(rotation));
}

void Transform::setRotation(Quaternion rotation)
{
	_rotation = rotation;
	_hasChangedLastFrame = true;
}

void Transform::setScale(Vector3f scale)
{
	if (!scale.equals(_scale))
	{
		_scale = scale;
		_hasChangedLastFrame = true;
	}
}

void Transform::setParent(Transform *parent)
{
	if (_parent != nullptr)
	{
		_parent->removeChild(this);
	}

	_parent = parent;

	if (parent != nullptr)
		parent->children.push_back(this);
}

void Transform::setParent(Entity *_entity)
{
	setParent(_entity->transform);
}

void Transform::rotate(Quaternion quat)
{
	_rotation = (quat * _rotation).normalize();
	_hasChangedLastFrame = true;
}

Matrix4f Transform::getTransformationMatrix()
{
	return getTransformationMatrix(true, true, true);
}

Matrix4f Transform::getTransformationMatrix(bool rot, bool scale, bool trans)
{
	if (_parent != nullptr)
		return (_parent->getTransformationMatrix(rot, scale, trans)) * getLocalTransformationMatrix(rot, scale, trans);
	else
		return getLocalTransformationMatrix(rot, scale, trans);
}

Matrix4f Transform::getLocalTransformationMatrix()
{
	if (_hasChangedLastFrame)
	{
		_transformationMatrixCached = Matrix4f::IDENTITY;
		_transformationMatrixCached = getRotationMatrix() * Matrix4f::createScaling(_scale);

		_transformationMatrixCached(3, 0) = _position.x();
		_transformationMatrixCached(3, 1) = _position.y();
		_transformationMatrixCached(3, 2) = _position.z();

		_hasChangedLastFrame = false;
	}

	return _transformationMatrixCached;
}

Matrix4f Transform::getLocalTransformationMatrix(bool rot, bool scale, bool trans)
{
	if (rot && scale && trans)
		return getLocalTransformationMatrix();

	_transformationMatrixCached = Matrix4f::IDENTITY;

	if (rot)
		_transformationMatrixCached = _transformationMatrixCached * getRotationMatrix();

	if (scale)
		_transformationMatrixCached = _transformationMatrixCached * Matrix4f::createScaling(_scale);

	if (trans)
	{
		_transformationMatrixCached(3, 0) = _position.x();
		_transformationMatrixCached(3, 1) = _position.y();
		_transformationMatrixCached(3, 2) = _position.z();
	}

	_hasChangedLastFrame = true;

	return _transformationMatrixCached;
}
