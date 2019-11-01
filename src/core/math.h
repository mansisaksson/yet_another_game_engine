#pragma once
#include <cmath>

namespace math
{
	inline int rand_int()
	{
		return rand() % INT_MAX;
	}

	inline constexpr float pi()
	{
		return 3.14159265359f;
	}

	template<typename U>
	inline U deg_to_rad(const U& deg)
	{
		return deg * (pi() / 180.f);
	}

	inline bool nearly_equals(float a, float b, float epsilon = 0.001f)
	{
		return fabs(a - b) < epsilon;
	}

	inline bool nearly_zero(float a, float epsilon = 0.001f)
	{
		return nearly_equals(a, 0.f, epsilon);
	}
}