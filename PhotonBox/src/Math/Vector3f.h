#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <iostream>
#include <math.h>
#include "Vector2f.h"

class Vector3f
{
public:
	static const Vector3f ZERO;
	static const Vector3f ONE;

	static const Vector3f UNIT_X;
	static const Vector3f UNIT_Y;
	static const Vector3f UNIT_Z;

	static const Vector3f UP;
	static const Vector3f DOWN;
	static const Vector3f LEFT;
	static const Vector3f RIGHT;
	static const Vector3f FORWARD;
	static const Vector3f BACK;

	Vector3f() : _x(0.0f), _y(0.0f), _z(0.0f) {}
	Vector3f(float val) : _x(val), _y(val), _z(val) {}
	Vector3f(float x, float y, float z) : _x(x), _y(y), _z(z) {}

	float& x() { return _x; }
	float& y() { return _y; }
	float& z() { return _z; }

	float getX() const { return _x; }
	float getY() const { return _y; }
	float getZ() const { return _z; }

	void setX(float value) { _x = value; }
	void setY(float value) { _y = value; }
	void setZ(float value) { _z = value; }

	bool equals(const Vector3f& other)
	{
		return (_x == other._x && _y == other._y && _z == other._z);
	}

	float length()
	{
		return sqrt(_x * _x + _y * _y + _z * _z);
	}

	float lengthSqrd()
	{
		return _x * _x + _y * _y + _z * _z;
	}

	float distance(const Vector3f& other)
	{
		return (*this - other).length();
	}

	float distanceSqrd(const Vector3f& other)
	{
		return (*this - other).lengthSqrd();
	}

	inline Vector3f cross(const Vector3f& other) const
	{
		float x = _y * other.getZ() - _z * other.getY();
		float y = _z * other.getX() - _x * other.getZ();
		float z = _x * other.getY() - _y * other.getX();

		return Vector3f(x, y, z);
	}

	static Vector3f cross(const Vector3f& a, const Vector3f& b)
	{
		float x = a.getY() * b.getZ() - a.getZ() * b.getY();
		float y = a.getZ() * b.getX() - a.getX() * b.getZ();
		float z = a.getX() * b.getY() - a.getY() * b.getX();

		return Vector3f(x, y, z);
	}

	inline float dot(const Vector3f& other) const
	{
		return _x * other.getX() + _y * other.getY() + _z * other.getZ();
	}

	inline Vector3f normalize() const
	{
		Vector3f ret = *this;
		return ret / ret.length();
	}

	inline Vector2f xy() const
	{
		return Vector2f(_x, _y);
	}

	inline bool operator== (const Vector3f& other) const
	{
		return (_x == other._x && _y == other._y && _z == other._z);
	}

	inline bool operator!= (const Vector3f& other) const
	{
		return (_x != other._x && _y != other._y && _z != other._z);
	}

	inline Vector3f operator/ (const float& scalar) const
	{
		return Vector3f(_x / scalar, _y / scalar, _z / scalar);
	}

	inline Vector3f operator* (const float& scalar) const
	{
		return Vector3f(_x * scalar, _y * scalar, _z * scalar);
	}

	inline Vector3f operator- (const Vector3f& other) const
	{
		return Vector3f(_x - other._x, _y - other._y, _z - other._z);
	}

	inline Vector3f operator+ (const Vector3f& other) const
	{
		return Vector3f(_x + other._x, _y + other._y, _z + other._z);
	}

	float& operator [](int index)
	{
		if (index == 0) return _x;
		else if (index == 1) return _y;
		else if (index == 2) return _z;
	}

	float operator [](int index) const
	{
		if (index == 0) return _x;
		else if (index == 1) return _y;
		else if (index == 2) return _z;
	}

	static Vector3f lerp(Vector3f start, Vector3f target, float percent);

	friend std::ostream& operator<<(std::ostream& os, Vector3f m)
	{
		return os << "(" << m.x() << ", " << m.y() << ", " << m.z() << ")";
	}

private:
	float _x, _y, _z;
};

#endif // VECTOR3F_H
