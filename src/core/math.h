#pragma once
#include <cmath>

namespace math
{
	inline int rand_int()
	{
		return rand() % INT_MAX;
	}

	inline float pi()
	{
		return 3.14159265359f;
	}

	/*bool nearly_equals(float a, float b, float epsilon = 0.001)
	{
		return fabs(a - b) < epsilon;
	}*/

	/*bool nearly_equals(double a, double b, double epsilon = 0.001)
	{
		return fabs(a - b) < epsilon;
	}*/
}