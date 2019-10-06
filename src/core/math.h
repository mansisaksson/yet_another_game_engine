#pragma once
#include <cmath>

namespace math
{
	inline int rand_int()
	{
		return rand() % INT_MAX;
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