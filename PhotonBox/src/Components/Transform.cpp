#include <algorithm>
#include "../Core/Entity.h"
#include "Camera.h"
#include "Transform.h"
#include "../Core/Systems/DebugGUI.h"
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
	bool temp = _hasChanged;
	_hasChanged = false;
	return temp;
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
		ImGui::SliderFloat3(entity->name.c_str(), &(_position[0]), -10, 10);
/*
		ImGui::Text("Some Text");

ImGui::Begin(entity->name.c_str());
		ImGui::SliderFloat3(entity->name.c_str(), &(_position[0]), 0, 1);
		if (ImGui::Button("Button"))
		{
			_hasChangedLastFrame = true;
			_hasChanged = true;
		}
		ImGui::End();
		*/
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
	return Matrix4f::createRotation(_rotation.z(), Vector3f::UNIT_Z) * Matrix4f::createRotation(_rotation.x(), Vector3f::UNIT_X) * Matrix4f::createRotation(_rotation.y(), Vector3f::UNIT_Y);
}

Vector3f Transform::getRotation()
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
		_hasChanged = true;
	}
}

void Transform::setRotation(Vector3f rotation)
{
	_rotation = rotation;
	_hasChangedLastFrame = true;
	_hasChanged = true;
}

void Transform::setScale(Vector3f scale)
{
	if (!scale.equals(_scale))
	{
		_scale = scale;
		_hasChangedLastFrame = true;
		_hasChanged = true;
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

	_hasChanged = true;
}

void Transform::setParent(Entity *_entity)
{
	setParent(_entity->transform);
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
		_modelMatrixCached = Matrix4f::IDENTITY;

		_modelMatrixCached = getRotationMatrix() * Matrix4f::createScaling(_scale);

		_modelMatrixCached(3, 0) = _position.x();
		_modelMatrixCached(3, 1) = _position.y();
		_modelMatrixCached(3, 2) = _position.z();

		_hasChangedLastFrame = false;
	}

	return _modelMatrixCached;
}

Matrix4f Transform::getLocalTransformationMatrix(bool rot, bool scale, bool trans)
{
	if (rot && scale && trans)
		return getLocalTransformationMatrix();

	_modelMatrixCached = Matrix4f::IDENTITY;

	if (rot)
		_modelMatrixCached = _modelMatrixCached * getRotationMatrix();

	if (scale)
		_modelMatrixCached = _modelMatrixCached * Matrix4f::createScaling(_scale);

	if (trans)
	{
		_modelMatrixCached(3, 0) = _position.x();
		_modelMatrixCached(3, 1) = _position.y();
		_modelMatrixCached(3, 2) = _position.z();
	}

	_hasChangedLastFrame = true;

	return _modelMatrixCached;
}
