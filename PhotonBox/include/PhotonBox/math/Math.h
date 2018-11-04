#ifndef MATH_H
#define MATH_H

#include <algorithm>
#include <cstdint>

#define PI 3.14159265358979323846

/***
RIGHT HANDED SYSTEM!
***/
namespace Math
{
	static float lerp(float a, float b, float percent)
	{
		return a + percent * (b - a);
	}


	static Vector3f toEulerAngle(const physx::PxQuat& q)
	{
		float roll, pitch, yaw;

		// roll (x-axis rotation)
		double sinr = +2.0 * (q.w * q.x + q.y * q.z);
		double cosr = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
		roll = (float)atan2(sinr, cosr);

		// pitch (y-axis rotation)
		/*
		float m = (std::max)(-2.0f * (q.x * q.z - q.w * q.y), -1.0f);
		float t = (std::min)(m, 1.0f);
		pitch = asin(t);
		*/
		
		double sinp = +2.0 * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1)
			pitch = (float)copysign(PI / 2, sinp); // use 90 degrees if out of range
		else
			pitch = (float)asin(sinp);
		

		// yaw (z-axis rotation)
		double siny = +2.0 * (q.w * q.z + q.x * q.y);
		double cosy = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
		yaw = (float)atan2(siny, cosy);

		return Vector3f(roll, pitch, yaw);
	}


	static Vector3f toEulerAngle(Quaternion q)
	{
		float roll, pitch, yaw;

		// roll (x-axis rotation)
		double sinr = +2.0 * (q.getW() * q.getX() + q.getY() * q.getZ());
		double cosr = +1.0 - 2.0 * (q.getX() * q.getX() + q.getY() * q.getY());
		roll = (float)atan2(sinr, cosr);

		// pitch (y-axis rotation)
		float m = (std::max)(-2.0f * (q.getX() * q.getZ() - q.getW() * q.getY()), -1.0f);
		float t = (std::min)(m, 1.0f);
		pitch = (float)asin(t);

		/*
		double sinp = +2.0 * (q.getW() * q.getY() - q.getZ() * q.getX());
		if (fabs(sinp) >= 1)
		pitch = copysign(PI / 2, sinp); // use 90 degrees if out of range
		else
		pitch = asin(sinp);
		*/

		// yaw (z-axis rotation)
		double siny = +2.0 * (q.getW()* q.getZ() + q.getX()* q.getY());
		double cosy = +1.0 - 2.0 * (q.getY()* q.getY() + q.getZ()* q.getZ());
		yaw = (float)atan2(siny, cosy);

		return Vector3f(roll, pitch, yaw);
	}
}

#endif // MATH_H
