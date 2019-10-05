#pragma once
#include <cmath>

namespace math
{
	inline int rand_int()
	{
		return rand() % INT_MAX;
	}
}