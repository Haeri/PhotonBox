#ifndef MATH_H
#define MATH_H

#include <algorithm>
#include <cstdint>

#include "PhotonBox/math/Vector3f.h"
#include "PhotonBox/math/Quaternion.h"

#define PI 3.14159265358979323846

/***
RIGHT HANDED SYSTEM!
***/
namespace Math
{
	static const Vector2f halton_seq_16[16] = {
		Vector2f(0.00398f, 0.99203f),
		Vector2f(0.49402f, 0.66002f),
		Vector2f(0.24501f, 0.32802f),
		Vector2f(0.74302f, 0.88136f),
		Vector2f(0.12051f, 0.54935f),
		Vector2f(0.61852f, 0.21735f),
		Vector2f(0.36952f, 0.77069f),
		Vector2f(0.86752f, 0.43868f),
		Vector2f(0.05826f, 0.10668f),
		Vector2f(0.55627f, 0.95514f),
		Vector2f(0.30727f, 0.62313f),
		Vector2f(0.80527f, 0.29113f),
		Vector2f(0.18276f, 0.84447f),
		Vector2f(0.68077f, 0.51246f),
		Vector2f(0.43177f, 0.18046f),
		Vector2f(0.92978f, 0.73380f),
	};

	static float lerp(float a, float b, float percent)
	{
		return a + percent * (b - a);
	}

	template<typename T>
	static T min(T a, T b)
	{
		return (((a) < (b)) ? (a) : (b));
	}

	template<typename T>
	static T max(T a, T b)
	{
		return (((a) > (b)) ? (a) : (b));
	}

	template<typename T>
	static T clamp(T value, T min, T max)
	{
		return Math::min(Math::max(value, min), max);
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
		float m = max(-2.0f * (q.getX() * q.getZ() - q.getW() * q.getY()), -1.0f);
		float t = min(m, 1.0f);
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
