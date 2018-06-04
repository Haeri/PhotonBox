#include "Quaternion.h"
#include <math.h>

const Quaternion Quaternion::ZERO = Quaternion(0, 0, 0, 0);

Quaternion::Quaternion(Vector3f axis, float angle)
{
	float sinHalfAngle = (float)sin(angle / 2);
	float cosHalfAngle = (float)cos(angle / 2);

	_quaternion = Vector4f(axis.getX() * sinHalfAngle, axis.getY() * sinHalfAngle, axis.getZ() * sinHalfAngle, cosHalfAngle);
}

float Quaternion::length()
{
	return _quaternion.length();
}

float Quaternion::lengthSqrd()
{
	return _quaternion.lengthSqrd();
}

float Quaternion::getAngle()
{
	return _quaternion.getW();
}

Vector3f Quaternion::getAxis()
{
	return Vector3f(_quaternion.getX(), _quaternion.getY(), _quaternion.getZ());
}

Matrix4f Quaternion::createRotation()
{
	/*
	Vector3f forward = new Vector3f(2.0f * (m_x * m_z - m_w * m_y), 2.0f * (m_y * m_z + m_w * m_x), 1.0f - 2.0f * (m_x * m_x + m_y * m_y));
	Vector3f up = new Vector3f(2.0f * (m_x * m_y + m_w * m_z), 1.0f - 2.0f * (m_x * m_x + m_z * m_z), 2.0f * (m_y * m_z - m_w * m_x));
	Vector3f right = new Vector3f(1.0f - 2.0f * (m_y * m_y + m_z * m_z), 2.0f * (m_x * m_y - m_w * m_z), 2.0f * (m_x * m_z + m_w * m_y));

	return new Matrix4f().InitRotation(forward, up, right);
	*/
	return Matrix4f::IDENTITY;
}

inline Quaternion Quaternion::operator*(const Quaternion & other) const
{
	float w_ = _quaternion.getW() * other.getW() - _quaternion.getX() * other.getX() - _quaternion.getY() * other.getY() - _quaternion.getZ() * other.getZ();
	float x_ = _quaternion.getX() * other.getW() + _quaternion.getW() * other.getX() + _quaternion.getY() * other.getZ() - _quaternion.getZ() * other.getY();
	float y_ = _quaternion.getY() * other.getW() + _quaternion.getW() * other.getY() + _quaternion.getZ() * other.getX() - _quaternion.getX() * other.getZ();
	float z_ = _quaternion.getZ() * other.getW() + _quaternion.getW() * other.getZ() + _quaternion.getX() * other.getY() - _quaternion.getY() * other.getX();

	return Quaternion(x_, y_, z_, w_);
}

inline Quaternion Quaternion::operator*(const float & scalar) const
{
	return Quaternion(_quaternion.getX() * scalar, _quaternion.getY() * scalar, _quaternion.getZ() * scalar, _quaternion.getW() * scalar);
}

