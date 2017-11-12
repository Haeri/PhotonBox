#include "Camera.h"

Camera* Camera::_main;

Camera::Camera() {
	if (_main == nullptr) setMain();

	float aspect = (float)Display::getWidth() / (float)Display::getHeight();
	setProjection(70, aspect, 0.01f, 1000.0f);
}

void Camera::updateAspect() {
	_aspect = (float)Display::getWidth() / (float)Display::getHeight();
	updateProjection();
}

void Camera::setFOV(float fov) {
	_fov = fov;
	updateProjection();
}

void Camera::updateProjection() {
	_projection = Matrix4f::createPerspective(_fov, _aspect, _zNear, _zFar);
}

void Camera::setProjection(float fov, float aspect, float zNear, float zFar) {
	_fov = fov;
	_aspect = aspect;
	_zNear = zNear;
	_zFar = zFar;
	updateProjection();
}

Matrix4f Camera::getViewMatrix() {
	return Matrix4f::lookAt(transform->getPositionWorld(), transform->up(), transform->forward());
}

//TODO: cache this matrix
Matrix4f Camera::getViewProjection() {
	return _projection * Matrix4f::lookAt(transform->getPositionWorld(), transform->up(), transform->forward());
}

Vector2f Camera::worldToScreen(Vector3f point) {
	Vector4f clipSpacePos = Camera::getMainCamera()->getViewProjection() * Vector4f(point, 1.0);
	if (clipSpacePos.w() <= 0) {
		clipSpacePos.w() = 0.0001;
	}

	return Vector2f(clipSpacePos.x() / clipSpacePos.w(), clipSpacePos.y() / clipSpacePos.w());
}

void Camera::setMain() {
	_main = this;
}

void Camera::destroy() {
	if (_main == this) _main = nullptr;
}