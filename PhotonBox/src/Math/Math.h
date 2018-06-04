#ifndef MATH_H
#define MATH_H

#define PI 3.14159265358979323846

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
		roll = atan2(sinr, cosr);

		// pitch (y-axis rotation)
		double sinp = +2.0 * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1)
			pitch = copysign(PI / 2, sinp); // use 90 degrees if out of range
		else
			pitch = asin(sinp);

		// yaw (z-axis rotation)
		double siny = +2.0 * (q.w * q.z + q.x * q.y);
		double cosy = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
		yaw = atan2(siny, cosy);

		return Vector3f(roll, pitch, yaw);
	}
}

#endif // MATH_H