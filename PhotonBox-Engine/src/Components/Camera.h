#ifndef CAMERA_H
#define CAMERA_H

#include "Behaviour.h"
#include "Transform.h"
#include "../Math/Matrix4f.h"
#include "../Core/Display.h"
#include "../Math/Vector2f.h"

class Camera: public Behaviour{
public:
	void Start() {
		if (_main == nullptr) setMain();

		float aspect = (float)Display::getWidth() / (float)Display::getHeight();
		setProjection(45, aspect, 0.1f, 50.0f);
	}

	void updateAspect() {
		_aspect = (float)Display::getWidth() / (float)Display::getHeight();
		updateProjection();
	}

	void setFOV(float fov) {
		fov = fov;
		updateProjection();
	}

	void updateProjection() {
		_projection = Matrix4f::createPerspective(_fov, _aspect, _zNear, _zFar);
	}

	void setProjection(float fov, float aspect, float zNear, float zFar) {
		_fov = fov;
		_aspect = aspect;
		_zNear = zNear;
		_zFar = zFar;
		updateProjection();
	}

	Matrix4f getProjectionMatrix() {
		return _projection;
	}

	Matrix4f getViewProjection() {
		return _projection * Matrix4f::lookAt(transform->getPositionWorld(), transform->up(), transform->forward());
	}

	static Vector2f worldToScreen(Vector3f point) {
		Vector4f clipSpacePos = Camera::getMainCamera()->getViewProjection() * Vector4f(point, 1.0);
		if (clipSpacePos.w() == 0) {
			clipSpacePos.w() = 0.0001;
		}

		return Vector2f(clipSpacePos.x() / clipSpacePos.w(), clipSpacePos.y() / clipSpacePos.w());
	}

	void setMain() {
		_main = this;
	}

	static Camera* getMainCamera() {
		return _main;
	}

private:
	static Camera* _main;
	float _fov, _zNear, _zFar, _aspect;
	Matrix4f _projection;
};
#endif /* defined(CAMERA_H) */
