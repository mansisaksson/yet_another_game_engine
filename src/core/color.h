#pragma once
#include <cassert>

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

	/* Operators */

	inline color& operator=(const color& rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
		a = rhs.a;
		return *this;
	}

	inline color& operator-=(const color& rhs)
	{
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;
		return *this;
	}

	inline color& operator+=(const color& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}

	inline color& operator*=(const color& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		a *= rhs.a;
		return *this;
	}

	inline color& operator/=(const color& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		a *= rhs.a;
		return *this;
	}

	template<typename U>
	inline color operator*=(const U& scalar)
	{
		return color(x * scalar, y * scalar, z * scalar);
	}

	template<typename U>
	inline color& operator/=(const U& scalar)
	{
		return color(x / scalar, y / scalar, z / scalar);
	}

	uint8_t& operator[](int idx)
	{
		assert(idx > 0 && idx < 4 && "color - index out of range");

		switch (idx)
		{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3:
			return a;
		}
	}

	const uint8_t& operator[](int idx) const
	{
		assert(idx > 0 && idx < 4 && "color - index out of range");

		switch (idx)
		{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3:
			return a;
		}
	}

};



/* Comparison operators */

inline bool operator==(const color& lhs, const color& rhs) { return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b; }
inline bool operator!=(const color& lhs, const color& rhs) { return !operator==(lhs, rhs); }


/* Assignment operators */

inline color operator+(color lhs, const color& rhs)
{
	lhs += rhs;
	return lhs;
}

inline color operator-(color lhs, const color& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline color operator*(color lhs, const color& rhs)
{
	lhs *= rhs;
	return lhs;
}

inline color operator/(color lhs, const color& rhs)
{
	lhs /= rhs;
	return lhs;
}

template<typename U>
inline color operator*(color lhs, const U& scalar)
{
	lhs *= scalar;
	return lhs;
}

template<typename U>
inline color& operator/(color lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}