#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <math.h>

class Vector3f {
public:
	float x, y, z;

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

	Vector3f(float x, float y, float z): x(x), y(y), z(z) {}


	bool equals(const Vector3f& other) {
		return (x == other.x && y == other.y && z == other.z);
	}

	float length() {
		return sqrt(x * x + y * y + z * z);
	}

	float lengthSqrd() {
		return x * x + y * y + z * z;
	}

	float distance(const Vector3f& other) {
		return (*this - other).length();
	}

	float distanceSqrd(const Vector3f& other) {
		return (*this - other).lengthSqrd();
	}

	inline Vector3f normalize() const {
		Vector3f ret = *this;
		return ret / ret.length();
	}

	inline bool operator== (const Vector3f& other) const {
		return (x == other.x && y == other.y && z == other.z);
	}

	inline Vector3f operator/ (const float& scalar) const {
		return Vector3f(x / scalar, y / scalar, z / scalar);
	}

	inline Vector3f operator* (const float& scalar) const {
		return Vector3f(x * scalar, y * scalar, z * scalar);
	}

	inline Vector3f operator- (const Vector3f& other) const{
		return Vector3f(x - other.x, y - other.y, z - other.z);
	}

	inline Vector3f operator+ (const Vector3f& other) const{
		return Vector3f(x + other.x, y + other.y, z + other.z);
	}
private:
};
#endif /* defined(VECTOR3F_H) */
