#ifndef QUATERNION_H
#define QUATERNION_H

#include "PhotonBox/math/Vector4f.h"
#include "PhotonBox/math/Matrix4f.h"

class Quaternion
{
public:
	static const Quaternion ZERO;

	Quaternion()
		: _x(0), _y(0), _z(0), _w(0) {}
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

	static Quaternion lerp(Quaternion start, Quaternion end, float percent);

	Matrix4f createRotation();

	Quaternion operator* (const Quaternion& scalar);
	inline Quaternion operator* (const float& scalar);

	float& operator [](int index)
	{
		if (index == 0) return _x;
		else if (index == 1) return _y;
		else if (index == 2) return _z;
		else if (index == 3) return _w;
		else
		{
			std::cerr << "index " << std::to_string(index) << " out of range\n";
			return _w;
		}
	}

	float operator [](int index) const
	{
		if (index == 0) return _x;
		else if (index == 1) return _y;
		else if (index == 2) return _z;
		else if (index == 3) return _w;
		else
		{
			std::cerr << "index " << std::to_string(index) << " out of range\n";
			return _w;
		}
	}

	friend std::ostream& operator<<(std::ostream& os, Quaternion m)
	{
		return os << "(" << m.getX() << ", " << m.getY() << ", " << m.getZ() << ", " << m.getW() << ")";
	}
private:
	float _x, _y, _z, _w;
};

#endif // MATRIX4F_H
