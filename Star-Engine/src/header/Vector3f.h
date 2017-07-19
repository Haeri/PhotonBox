#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <iostream>
#include <math.h>

class Vector3f {
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

	Vector3f(float x, float y, float z): _x(x), _y(y), _z(z) {}

	float x() { return _x; }
	float y() { return _y; }
	float z() { return _z; }

	void setX(float value) { _x = value; }
	void setY(float value) { _y = value; }
	void setZ(float value) { _z = value; }

	bool equals(const Vector3f& other) {
		return (_x == other._x && _y == other._y && _z == other._z);
	}

	float length() {
		return sqrt(_x * _x + _y * _y + _z * _z);
	}

	float lengthSqrd() {
		return _x * _x + _y * _y + _z * _z;
	}

	float distance(const Vector3f& other) {
		return (*this - other).length();
	}

	float distanceSqrd(const Vector3f& other) {
		return (*this - other).lengthSqrd();
	}

	inline Vector3f normali_ze() const {
		Vector3f ret = *this;
		return ret / ret.length();
	}

	inline bool operator== (const Vector3f& other) const {
		return (_x == other._x && _y == other._y && _z == other._z);
	}

	inline Vector3f operator/ (const float& scalar) const {
		return Vector3f(_x / scalar, _y / scalar, _z / scalar);
	}

	inline Vector3f operator* (const float& scalar) const {
		return Vector3f(_x * scalar, _y * scalar, _z * scalar);
	}

	inline Vector3f operator- (const Vector3f& other) const{
		return Vector3f(_x - other._x, _y - other._y, _z - other._z);
	}

	inline Vector3f operator+ (const Vector3f& other) const{
		return Vector3f(_x + other._x, _y + other._y, _z + other._z);
	}

	float operator [](int index) const {
		if (index == 0) return _x;
		else if (index == 1) return _y;
		else if (index == 2) return _z;
		else {
			std::cerr << "Error: Index " << index << " out of range!" << std::endl;
			return -1;
		}
	}

	friend std::ostream& operator<<(std::ostream& os, Vector3f m) {
		return os << "(" << m.x() << ", " << m.y() << ", " << m.z() << ")";
	}

private:
	float _x, _y, _z;
};

#endif /* defined(VECTOR3F_H) */
