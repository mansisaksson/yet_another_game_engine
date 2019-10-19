#pragma once
#include "glm/glm.hpp"

class color
{
public:
	uint8_t r, g, b, a;

	color()
		: r(0)
		, g(0)
		, b(0)
		, a(1)
	{}

	color(uint8_t t_r, uint8_t t_g, uint8_t t_b, uint8_t t_a = 255)
		: r(t_r)
		, g(t_g)
		, b(t_b)
		, a(t_a)
	{}

	color(const color&o)
		: r(o.r)
		, g(o.g)
		, b(o.b)
		, a(o.a)
	{}

	static const color red;
	static const color green;
	static const color blue;

	/* ~Begin: Operators */

	inline bool operator==(const color& o) const
	{
		return r == o.r && g == o.g && b == o.b && a == o.a;
	}

	inline color operator=(const color& o) const
	{
		return o;
	}

	color operator-(const color& o)
	{
		return color(r - o.r, g - o.g, b - o.b, a - o.a);
	}

	color operator+(const color& o)
	{
		return color(r + o.r, g + o.g, b + o.b, a + o.a);
	}

	void operator-=(const color& o)
	{
		r -= o.r;
		g -= o.g;
		b -= o.b;
	}

	void operator+=(const color& o)
	{
		r += o.b;
		g += o.g;
		b += o.b;
	}

	template<typename U>
	color operator*(const U& scalar)
	{
		return color(r * scalar, g * scalar, b * scalar, a * scalar);
	}

	template<typename U>
	color operator/(const U& scalar)
	{
		return color(r / scalar, g / scalar, b / scalar, a / scalar);
	}

	/* ~End: Operators */


};