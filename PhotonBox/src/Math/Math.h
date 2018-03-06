#ifndef MATH_H
#define MATH_H

#define PI 3.14159265358979323846

namespace Math
{
	float lerp(float a, float b, float percent)
	{
		return a + percent * (b - a);
	}
}

#endif // MATH_H
