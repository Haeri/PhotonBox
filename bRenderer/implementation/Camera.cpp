#include "headers/Camera.h"
#include "headers/Configuration.h"
#include <cassert>

/* Constructor and Destructor */

Camera::Camera()
	: Camera(bRenderer::DEFAULT_CAMERA_POSITION(), bRenderer::DEFAULT_CAMERA_ROTATION_AXES(), bRenderer::DEFAULT_FIELD_OF_VIEW(), bRenderer::DEFAULT_ASPECT_RATIO(), bRenderer::DEFAULT_NEAR_CLIPPING_PLANE(), bRenderer::DEFAULT_FAR_CLIPPING_PLANE())
{}

Camera::Camera(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
	: Camera(bRenderer::DEFAULT_CAMERA_POSITION(), bRenderer::DEFAULT_CAMERA_ROTATION_AXES(), fov, aspect, near, far)
{}

Camera::Camera(const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes)
	: Camera(position, rotationAxes, bRenderer::DEFAULT_FIELD_OF_VIEW(), bRenderer::DEFAULT_ASPECT_RATIO(), bRenderer::DEFAULT_NEAR_CLIPPING_PLANE(), bRenderer::DEFAULT_FAR_CLIPPING_PLANE())
{}

Camera::Camera(const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
	: _position(position), _rotationAxes(rotationAxes), _fov(fov), _aspect(aspect), _near(near), _far(far)
{}

/* Public Functions */

void Camera::moveCameraForward(GLfloat camSpeed)
{
	_position -= camSpeed * getForward();
}

void Camera::moveCameraSideward(GLfloat camSpeed)
{
	_position -= camSpeed * getRight();
}

void Camera::moveCameraUpward(GLfloat camSpeed)
{
	_position -= camSpeed * getUp();
}

void Camera::rotateCamera(GLfloat rotationX, GLfloat rotationY, GLfloat rotationZ)
{
	_rotationAxes[0] -= rotationX;
	_rotationAxes[1] -= rotationY;
	_rotationAxes[2] -= rotationZ;
}

void Camera::resetCamera()
{
	_position = bRenderer::DEFAULT_CAMERA_POSITION();
	_rotationAxes = bRenderer::DEFAULT_CAMERA_ROTATION_AXES();
}

vmml::Matrix4f Camera::getViewMatrix(){
	return getRotation() * vmml::create_translation(getPosition());
}

vmml::Matrix4f Camera::getInverseViewMatrix(){
	return vmml::create_translation(-getPosition()) * getInverseRotation();
}

vmml::Matrix4f Camera::getProjectionMatrix()
{
	return createPerspective(_fov, _aspect, _near, _far);
}

vmml::Matrix4f Camera::getRotation()
{
	return  vmml::create_rotation(_rotationAxes[2], vmml::Vector3f::UNIT_Z) * vmml::create_rotation(_rotationAxes[0], vmml::Vector3f::UNIT_X) * vmml::create_rotation(_rotationAxes[1], vmml::Vector3f::UNIT_Y);
}

vmml::Matrix4f Camera::getInverseRotation()
{
	vmml::Matrix4f rotInv = vmml::Matrix4f::IDENTITY;
	getRotation().transpose_to(rotInv);
	return rotInv;
}

vmml::Matrix4f Camera::getInverseRotationX()
{
	vmml::Matrix4f rotInv = vmml::Matrix4f::IDENTITY;
	(vmml::create_rotation(_rotationAxes[0], vmml::Vector3f::UNIT_X)).transpose_to(rotInv);
	return rotInv;
}

vmml::Matrix4f Camera::getInverseRotationY()
{
	vmml::Matrix4f rotInv = vmml::Matrix4f::IDENTITY;
	(vmml::create_rotation(_rotationAxes[1], vmml::Vector3f::UNIT_Y)).transpose_to(rotInv);
	return rotInv;
}

vmml::Matrix4f Camera::getInverseRotationZ()
{
	vmml::Matrix4f rotInv = vmml::Matrix4f::IDENTITY;
	(vmml::create_rotation(_rotationAxes[2], vmml::Vector3f::UNIT_Z)).transpose_to(rotInv);
	return rotInv;
}

vmml::Vector3f Camera::getForward()
{
	vmml::Matrix4f r = getRotation();
	return vmml::Vector3f(r.at(2,0), r.at(2,1), r.at(2,2));
}

vmml::Vector3f Camera::getRight()
{
	vmml::Matrix4f r = getRotation();
	return vmml::Vector3f(r.at(0, 0), r.at(0, 1), r.at(0, 2));
}

vmml::Vector3f Camera::getUp()
{
	vmml::Matrix4f r = getRotation();
	return vmml::Vector3f(r.at(1, 0), r.at(1, 1), r.at(1, 2));
}

/* Static Functions */

vmml::Matrix4f Camera::lookAt(const vmml::Vector3f &eye, const vmml::Vector3f &target, const vmml::Vector3f &up){
	vmml::Vector3f zaxis = vmml::normalize(eye - target);
	vmml::Vector3f xaxis = vmml::normalize(vmml::cross<3>(up, zaxis));
	vmml::Vector3f yaxis = vmml::cross<3>(zaxis, xaxis);

	vmml::Matrix4f view;
	view.set_row(0, vmml::Vector4f(xaxis.x(), xaxis.y(), xaxis.z(), -vmml::dot(xaxis, eye)));
	view.set_row(1, vmml::Vector4f(yaxis.x(), yaxis.y(), yaxis.z(), -vmml::dot(yaxis, eye)));
	view.set_row(2, vmml::Vector4f(zaxis.x(), zaxis.y(), zaxis.z(), -vmml::dot(zaxis, eye)));
	view.set_row(3, vmml::Vector4f(0, 0, 0, 1.0));

	return view;
}

vmml::Matrix4f Camera::createPerspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	vmml::Matrix4f perspective = vmml::Matrix4f::IDENTITY;

	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			perspective.at(i, j) = 0.0f;
		}
	}

	GLfloat angle = (fov / 180.0f) * M_PI_F;
	GLfloat f = 1.0f / tan(angle * 0.5f);

	perspective.at(0, 0) = f / aspect;
	perspective.at(1, 1) = f;
	perspective.at(2, 2) = (far + near) / (near - far);
	perspective.at(2, 3) = -1.0f;
	perspective.at(3, 2) = (2.0f * far*near) / (near - far);


	return perspective;
}