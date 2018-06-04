#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector4f.h"
#include "Matrix4f.h"

class Quaternion
{
public:
	static const Quaternion ZERO;

	Quaternion(float x, float y, float z, float w) : _quaternion(x, y, z, w) {}
	Quaternion(Vector3f axis, float angle);

	float getX() const { return _quaternion.getX(); }
	float getY() const { return _quaternion.getY(); }
	float getZ() const { return _quaternion.getZ(); }
	float getW() const { return _quaternion.getW(); }

	float length();
	float lengthSqrd();
	float getAngle();
	Vector3f getAxis();

	Matrix4f createRotation();

	inline Quaternion operator* (const Quaternion& scalar) const;
	inline Quaternion operator* (const float& scalar) const;
private:
	Vector4f _quaternion;
};

#endif // MATRIX4F_H