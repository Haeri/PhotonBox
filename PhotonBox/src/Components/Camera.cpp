#include "../Core/Entity.h"
#include "Camera.h"
#include "ObjectRenderer.h"

Camera* Camera::_main;

Camera::Camera()
{
	if (_main == nullptr) setMain();

	float aspect = (float)Display::getWidth() / (float)Display::getHeight();
	setPerspectiveProjection(70, aspect, 0.01f, 10000.0f);
}

void Camera::updateAspect()
{
	_aspect = (float)Display::getWidth() / (float)Display::getHeight();
	updateProjection();
}

void Camera::setFOV(float fov)
{
	_fov = fov;
	updateProjection();
}

void Camera::updateProjection()
{
	if(_isPerspective)
		_projection = Matrix4f::createPerspective(_fov, _aspect, _zNear, _zFar);
	else
		_projection = Matrix4f::createOrthographic(-_radius * _aspect, _radius * _aspect, -_radius, _radius, _zNear, _zFar);
}

void Camera::setOrhographicProjection(float radius, float zNear, float zFar)
{
	_isPerspective = false;
	_radius = radius;
	_zNear = zNear;
	_zFar = zFar;
	updateProjection();
}

void Camera::setPerspectiveProjection(float fov, float aspect, float zNear, float zFar)
{
	_isPerspective = true;
	_fov = fov;
	_aspect = aspect;
	_zNear = zNear;
	_zFar = zFar;
	updateProjection();
}

Matrix4f Camera::getViewMatrix()
{
	if (transform->hasChanged())
	{
		_viewCache = Matrix4f::lookAt(transform->getPositionWorld(), transform->up(), transform->forward());
	}

	return _viewCache;
}

//TODO: cache this matrix
Matrix4f Camera::getViewProjection()
{
	return _projection * getViewMatrix();
}

Vector2f Camera::worldToScreen(Vector3f point)
{
	Vector4f clipSpacePos = Camera::getMainCamera()->getViewProjection() * Vector4f(point, 1.0);
	if (clipSpacePos.w() <= 0)
	{
		clipSpacePos.w() = 0.0001;
	}

	return Vector2f(clipSpacePos.x() / clipSpacePos.w(), clipSpacePos.y() / clipSpacePos.w());
}

void Camera::updateFrustum()
{
	//if (transform->hasChanged())
	{
	Matrix4f m = getViewProjection();
	//Left
	_frustum[0].normal.x()	= m(0, 3) + m(0, 0);
	_frustum[0].normal.y()	= m(1, 3) + m(1, 0);
	_frustum[0].normal.z()	= m(2, 3) + m(2, 0);
	_frustum[0].distance	= m(3, 3) + m(3, 0);
		
	//Right
	_frustum[1].normal.x()	= m(0, 3) - m(0, 0);
	_frustum[1].normal.y()	= m(1, 3) - m(1, 0);
	_frustum[1].normal.z()	= m(2, 3) - m(2, 0);
	_frustum[1].distance	= m(3, 3) - m(3, 0);

	// Top
	_frustum[2].normal.x()	= m(0, 3) - m(0, 1);
	_frustum[2].normal.y()	= m(1, 3) - m(1, 1);
	_frustum[2].normal.z()	= m(2, 3) - m(2, 1);
	_frustum[2].distance	= m(3, 3) - m(3, 1);

	//Bottom
	_frustum[3].normal.x()	= m(0, 3) + m(0, 1);
	_frustum[3].normal.y()	= m(1, 3) + m(1, 1);
	_frustum[3].normal.z()	= m(2, 3) + m(2, 1);
	_frustum[3].distance	= m(3, 3) + m(3, 1);

	//Near
	_frustum[4].normal.x()	= m(0, 3) + m(0, 2);
	_frustum[4].normal.y()	= m(1, 3) + m(1, 2);
	_frustum[4].normal.z()	= m(2, 3) + m(2, 2);
	_frustum[4].distance	= m(3, 3) + m(3, 2);

	//Far
	_frustum[5].normal.x()	= m(0, 3) - m(0, 2);
	_frustum[5].normal.y()	= m(1, 3) - m(1, 2);
	_frustum[5].normal.z()	= m(2, 3) - m(2, 2);
	_frustum[5].distance	= m(3, 3) - m(3, 2);

	// Normalize all plane normals
	for (int i = 0; i < 6; i++)
		_frustum[i].normal.normalize();
	}
}

void Camera::setMain()
{
	_main = this;
}

bool Camera::frustumTest(ObjectRenderer* object)
{
	updateFrustum();

	float maxScaleAxis = max(max(object->transform->getScale().x(), object->transform->getScale().y()), object->transform->getScale().z());
	Vector3f center = (object->transform->getTransformationMatrix() * Vector4f(object->getBoundingSphere().getCenter(), 0.0f)).xyz();
	
	/*
	std::cout << "LEFT:  " << std::to_string(center.dot(_frustum[0].normal) + _frustum[0].distance + maxScaleAxis * object->getBoundingSphere().getRadius()) << std::endl;
	std::cout << "RIGHT: " << std::to_string(center.dot(_frustum[1].normal) + _frustum[1].distance + maxScaleAxis * object->getBoundingSphere().getRadius()) << std::endl;
	std::cout << "TOP:   " << std::to_string(center.dot(_frustum[2].normal) + _frustum[2].distance + maxScaleAxis * object->getBoundingSphere().getRadius()) << std::endl;
	std::cout << "BOTTOM:" << std::to_string(center.dot(_frustum[3].normal) + _frustum[3].distance + maxScaleAxis * object->getBoundingSphere().getRadius()) << std::endl;
	std::cout << "NEAR:  " << std::to_string(center.dot(_frustum[4].normal) + _frustum[4].distance + maxScaleAxis * object->getBoundingSphere().getRadius()) << std::endl;
	std::cout << "FAR:   " << std::to_string(center.dot(_frustum[5].normal) + _frustum[5].distance + maxScaleAxis * object->getBoundingSphere().getRadius()) << std::endl;
	std::cout << std::endl;
	*/

	for (size_t i = 0; i < 6; ++i)
	{
		if(center.dot(_frustum[i].normal) + _frustum[i].distance + maxScaleAxis * object->getBoundingSphere().getRadius() <= 0)
		{
			return false;
		}
	}
	return true;
}

void Camera::destroy()
{
	if (_main == this) _main = nullptr;
}