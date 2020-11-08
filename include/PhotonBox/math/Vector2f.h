#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <iostream>
#include <math.h>
#include <string>

#include "PhotonBox/core/system/Physics.h"
#include "PhotonBox/util/Logger.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT Vector2f
{
public:

	static const Vector2f ZERO;
	static const Vector2f ONE;

	static const Vector2f UNIT_X;
	static const Vector2f UNIT_Y;

	static const Vector2f UP;
	static const Vector2f DOWN;
	static const Vector2f LEFT;
	static const Vector2f RIGHT;

	Vector2f() : _x(0.0f), _y(0.0f) {}
	Vector2f(float x, float y) : _x(x), _y(y) {}

	float& x() { return _x; }
	float& y() { return _y; }

	float getX() const { return _x; }
	float getY() const { return _y; }

	void setX(float value) { _x = value; }
	void setY(float value) { _y = value; }

	bool equals(const Vector2f& other)
	{
		return (_x == other._x && _y == other._y);
	}

	float length()
	{
		return sqrt(_x * _x + _y * _y);
	}

	float lengthSqrd()
	{
		return _x * _x + _y * _y;
	}

	float distance(const Vector2f& other)
	{
		return (*this - other).length();
	}

	float distanceSqrd(const Vector2f& other)
	{
		return (*this - other).lengthSqrd();
	}

	inline Vector2f normalize() const
	{
		Vector2f ret = *this;
		return ret / ret.length();
	}

	inline bool operator== (const Vector2f& other) const
	{
		return (_x == other._x && _y == other._y);
	}

	inline bool operator!= (const Vector2f& other) const
	{
		return (_x != other._x || _y != other._y);
	}

	inline Vector2f operator/ (const float& scalar) const
	{
		return Vector2f(_x / scalar, _y / scalar);
	}

	inline Vector2f operator* (const float& scalar) const
	{
		return Vector2f(_x * scalar, _y * scalar);
	}

	inline Vector2f operator- (const Vector2f& other) const
	{
		return Vector2f(_x - other._x, _y - other._y);
	}

	inline Vector2f operator+ (const Vector2f& other) const
	{
		return Vector2f(_x + other._x, _y + other._y);
	}

	float& operator [](int index)
	{
		if (index == 0) return _x;
		else if (index == 1) return _y;
	}

	float operator [](int index) const
	{
		if (index == 0) return _x;
		else if (index == 1) return _y;
	}

	static Vector2f lerp(Vector2f start, Vector2f target, float percent);

	friend std::ostream& operator<<(std::ostream& os, Vector2f m)
	{
		return os << "(" << m.x() << ", " << m.y() << ")";
	}

	// PHYSX
	physx::PxVec2 toPhysX()
	{
		return physx::PxVec2(_x, _y);
	}

private:
	float _x, _y;
};
#endif /* defined(VECTOR2F_H) */
