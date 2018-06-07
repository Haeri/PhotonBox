#include "Quaternion.h"
#include <math.h>

const Quaternion Quaternion::ZERO = Quaternion(0, 0, 0, 0);

Quaternion::Quaternion(Vector3f axis, float angle)
{
	float sinHalfAngle = (float)sin(angle / 2);
	float cosHalfAngle = (float)cos(angle / 2);

	_x = axis.getX() * sinHalfAngle;
	_y = axis.getY() * sinHalfAngle;
	_z = axis.getZ() * sinHalfAngle;
	_w = cosHalfAngle;
}

Quaternion::Quaternion(Vector3f euler)
{
	float sx = sin(euler.getX() * 0.5f);
	float sy = sin(euler.getY() * 0.5f);
	float sz = sin(euler.getZ() * 0.5f);
	
	float cx = cos(euler.getX() * 0.5f);
	float cy = cos(euler.getY() * 0.5f);
	float cz = cos(euler.getZ() * 0.5f);

	_w = cx * cy * cz + sx * sy * sz;
	_x = sx * cy * cz - cx * sy * sz;
	_y = cx * sy * cz + sx * cy * sz;
	_z = cx * cy * sz - sx * sy * cz;
}

float Quaternion::length()
{
	return sqrt(_x * _x + _y * _y + _z * _z + _w * _w);
}

float Quaternion::lengthSqrd()
{
	return _x * _x + _y * _y + _z * _z + _w * _w;
}

Quaternion Quaternion::normalize()
{
	float len = length();
	if (len <= 0) // Problem
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	float oneOverLen = 1.0f / len;
	return Quaternion(_x * oneOverLen, _y * oneOverLen, _z * oneOverLen, _w * oneOverLen);
}

float Quaternion::getAngle()
{
	return _w;
}

Vector3f Quaternion::getAxis()
{
	return Vector3f(_x, _y, _z);
}

Matrix4f Quaternion::createRotation()
{
	Matrix4f ret = Matrix4f::IDENTITY;
	float qxx(_x * _x);
	float qyy(_y * _y);
	float qzz(_z * _z);
	float qxz(_x * _z);
	float qxy(_x * _y);
	float qyz(_y * _z);
	float qwx(_w * _x);
	float qwy(_w * _y);
	float qwz(_w * _z);
	

	ret.set(0, 0, 1.0f - 2.0f * (qyy + qzz));
	ret.set(0, 1, 2.0f * (qxy + qwz));
	ret.set(0, 2, 2.0f * (qxz - qwy));

	ret.set(1, 0, 2.0f * (qxy - qwz));
	ret.set(1, 1, 1.0f - 2.0f * (qxx + qzz));
	ret.set(1, 2, 2.0f * (qyz + qwx));

	ret.set(2, 0, 2.0f * (qxz + qwy));
	ret.set(2, 1, 2.0f * (qyz - qwx));
	ret.set(2, 2, 1.0f - 2.0f * (qxx + qyy));

	return ret;

	/*
	Vector3f forward = new Vector3f(2.0f * (m_x * m_z - m_w * m_y), 2.0f * (m_y * m_z + m_w * m_x), 1.0f - 2.0f * (m_x * m_x + m_y * m_y));
	Vector3f up = new Vector3f(2.0f * (m_x * m_y + m_w * m_z), 1.0f - 2.0f * (m_x * m_x + m_z * m_z), 2.0f * (m_y * m_z - m_w * m_x));
	Vector3f right = new Vector3f(1.0f - 2.0f * (m_y * m_y + m_z * m_z), 2.0f * (m_x * m_y - m_w * m_z), 2.0f * (m_x * m_z + m_w * m_y));

	return new Matrix4f().InitRotation(forward, up, right);
	*/
	return Matrix4f::IDENTITY;
}

Quaternion Quaternion::operator*(const Quaternion & other)
{
	float w_ = _w * other.getW() - _x * other.getX() - _y * other.getY() - _z * other.getZ();
	float x_ = _x * other.getW() + _w * other.getX() + _y * other.getZ() - _z * other.getY();
	float y_ = _y * other.getW() + _w * other.getY() + _z * other.getX() - _x * other.getZ();
	float z_ = _z * other.getW() + _w * other.getZ() + _x * other.getY() - _y * other.getX();

	return Quaternion(x_, y_, z_, w_);
}

inline Quaternion Quaternion::operator*(const float & scalar)
{
	return Quaternion(_x * scalar, _y * scalar, _z * scalar, _w * scalar);
}

