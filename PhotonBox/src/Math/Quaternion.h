#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector4f.h"
#include "Matrix4f.h"

class Quaternion
{
public:
	static const Quaternion ZERO;

	Quaternion(float x, float y, float z, float w)
		: _x(x), _y(y), _z(z), _w(w) {}
	Quaternion(Vector3f axis, float angle);
	Quaternion(Vector3f euler);

	float getX() const { return _x; }
	float getY() const { return _y; }
	float getZ() const { return _z; }
	float getW() const { return _w; }

	float length();
	float lengthSqrd();
	Quaternion normalize();
	float getAngle();
	Vector3f getAxis();

	Matrix4f createRotation();

	Quaternion operator* (const Quaternion& scalar);
	inline Quaternion operator* (const float& scalar);
private:
	float _x, _y, _z, _w;
};

#endif // MATRIX4F_H