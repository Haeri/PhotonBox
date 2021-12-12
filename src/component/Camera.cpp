#include "PhotonBox/component/Camera.h"

#include "PhotonBox/core/Entity.h"
#include "PhotonBox/core/Display.h"
#include "PhotonBox/core/system/Logic.h"
#include "PhotonBox/component/ObjectRenderer.h"
#include "PhotonBox/math/Math.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Camera* Camera::_main;

void Camera::init()
{
	setMain();

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
		_projectionMatrix = Matrix4f::createPerspective(_fov, _aspect, _zNear, _zFar);
	else
		_projectionMatrix = Matrix4f::createOrthographic(-_radius * _aspect, _radius * _aspect, -_radius, _radius, _zNear, _zFar);
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

Matrix4f Camera::getProjectionMatrix()
{
	if (!_jitter) 
	{
		return _projectionMatrix;
	}
	else
	{
		Matrix4f jitterProjection;

		Vector2f offset = Math::halton_seq_16[Renderer::getFrameIndex() % 16];
		Vector4f extents = getProjectionExtents(offset.x(), offset.y());

		float cf = _zFar;
		float cn = _zNear;
		float xm = extents.z() - extents.x();
		float xp = extents.z() + extents.x();
		float ym = extents.w() - extents.y();
		float yp = extents.w() + extents.y();

		jitterProjection = Matrix4f::createPerspective(xm * cn, xp * cn, ym * cn, yp * cn, cn, cf);

		return jitterProjection;
	}
}

Matrix4f Camera::getViewMatrix()
{
	if (transform->hasChangedLastTick())
	{
		_viewMatrix = Matrix4f::lookAt(transform->getPositionWorld(), transform->up(), transform->forward());
	}

	return _viewMatrix;
}

Matrix4f Camera::getViewProjection()
{
	return getProjectionMatrix() * getViewMatrix();
}

Vector4f Camera::getProjectionExtents(float xOffset, float yOffset)
{
	float oneExtentY = tan(0.5f * (static_cast<float>(PI)/180.0f) * _fov);
	float oneExtentX = oneExtentY * _aspect;
	float texelSizeX = oneExtentX / (0.5f * Display::getWidth());
	float texelSizeY = oneExtentY / (0.5f * Display::getHeight());
	float oneJitterX = texelSizeX * xOffset;
	float oneJitterY = texelSizeY * yOffset;

	return Vector4f(oneExtentX, oneExtentY, oneJitterX, oneJitterY);// xy = frustum extents at distance 1, zw = jitter at distance 1
}

Vector2f Camera::worldToScreen(Vector3f point)
{
	Vector4f clipSpacePos = Camera::getMainCamera()->getViewProjection() * Vector4f(point, 1.0);
	if (clipSpacePos.w() <= 0)
	{
		clipSpacePos.w() = 0.0001f;
	}

	return Vector2f(clipSpacePos.x() / clipSpacePos.w(), clipSpacePos.y() / clipSpacePos.w());
}

void Camera::updateFrustum()
{
	if (transform->hasChangedSince(_lastFrustumUpdate))
	{
		_lastFrustumUpdate = Logic::getTickIndex();

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
		_frustum[4].normal.x()  = m(0, 3) + m(0, 2);
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
			_frustum[i].normalize();
	}
}

void Camera::setMain()
{
	_main = this;
}

bool Camera::frustumTest(ObjectRenderer* object)
{
	updateFrustum();

	float maxScaleAxis = Math::max(Math::max(object->getTransform()->getScale().x(), object->getTransform()->getScale().y()), object->getTransform()->getScale().z());
	Vector3f center = (object->getTransform()->getTransformationMatrix() * Vector4f(object->getBoundingSphere().getCenter(), 1.0f)).xyz();

	for (size_t i = 0; i < 5; ++i)
	{
		if(_frustum[i].normal.dot(center) + _frustum[i].distance + maxScaleAxis * object->getBoundingSphere().getRadius() < 0)
		{
			return false;
		}
	}
	return true;
}

void Camera::toggleJitter(bool jitter)
{
	_jitter = jitter;
}

void Camera::destroy()
{
	if (_main == this) _main = nullptr;
}
