#include "Camera.h"
#include "ObjectRenderer.h"
#include "../Core/GameObject.h"

Camera* Camera::_main;

Camera::Camera()
{
	if (_main == nullptr) setMain();

	float aspect = (float)Display::getWidth() / (float)Display::getHeight();
	setProjection(70, aspect, 0.01f, 10000.0f);
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
	_projection = Matrix4f::createPerspective(_fov, _aspect, _zNear, _zFar);
}

void Camera::setProjection(float fov, float aspect, float zNear, float zFar)
{
	_fov = fov;
	_aspect = aspect;
	_zNear = zNear;
	_zFar = zFar;
	updateProjection();
}

Matrix4f Camera::getViewMatrix()
{
	return Matrix4f::lookAt(transform->getPositionWorld(), transform->up(), transform->forward());
}

//TODO: cache this matrix
Matrix4f Camera::getViewProjection()
{
	return _projection * Matrix4f::lookAt(transform->getPositionWorld(), transform->up(), transform->forward());
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
	Matrix4f m = getViewProjection();
	_frustum[0].normal.x() = m(0, 3) - m(0, 0);
	_frustum[0].normal.y() = m(1, 3) - m(1, 0);
	_frustum[0].normal.z() = m(2, 3) - m(2, 0);
	_frustum[0].distance = m(3, 3) - m(3, 0);

	_frustum[1].normal.x() = m(0, 3) + m(0, 0);
	_frustum[1].normal.y() = m(1, 3) + m(1, 0);
	_frustum[1].normal.z() = m(2, 3) + m(2, 0);
	_frustum[1].distance = m(3, 3) + m(3, 0);

	_frustum[2].normal.x() = m(0, 3) + m(0, 1);
	_frustum[2].normal.y() = m(1, 3) + m(1, 1);
	_frustum[2].normal.z() = m(2, 3) + m(2, 1);
	_frustum[2].distance = m(3, 3) + m(3, 1);

	_frustum[3].normal.x() = m(0, 3) - m(0, 1);
	_frustum[3].normal.y() = m(1, 3) - m(1, 1);
	_frustum[3].normal.z() = m(2, 3) - m(2, 1);
	_frustum[3].distance = m(3, 3) - m(3, 1);

	_frustum[4].normal.x() = m(0, 3) - m(0, 2);
	_frustum[4].normal.y() = m(1, 3) - m(1, 2);
	_frustum[4].normal.z() = m(2, 3) - m(2, 2);
	_frustum[4].distance = m(3, 3) - m(3, 2);

	_frustum[5].normal.x() = m(0, 3) + m(0, 2);
	_frustum[5].normal.y() = m(1, 3) + m(1, 2);
	_frustum[5].normal.z() = m(2, 3) + m(2, 2);
	_frustum[5].distance = m(3, 3) + m(3, 2);

	// Normalize all plane normals
	for (int i = 0; i < 6; i++)
		_frustum[i].normal.normalize();
}

void Camera::setMain()
{
	_main = this;
}

bool Camera::frustumTest(ObjectRenderer* object)
{
	float maxScaleAxis = max(max(object->transform->getScale().x(), object->transform->getScale().y()), object->transform->getScale().z());

	for (size_t i = 0; i < 6; ++i)
	{
		if (object->transform->getPositionWorld().dot(_frustum[i].normal) + _frustum[i].distance + maxScaleAxis * object->getBoundingSphereRadius() <= 0)
		{
			std::cout << object->gameObject->name << std::endl;
			return false;
		}
	}

	return true;
}

void Camera::destroy()
{
	if (_main == this) _main = nullptr;
}