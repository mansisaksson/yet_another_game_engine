#pragma once
#include "string.h"

class vector2
{
public:
	float x, y;

	vector2()
		: x(0)
		, y(0)
	{}

	vector2(float t_x, float t_y)
		: x(t_x)
		, y(t_y)
	{}

	vector2(const vector2& o)
		: x(o.x)
		, y(o.y)
	{}


	/* statics */

	static const vector2 zero;
	static const vector2 one;


	/* vector math */

	void normalize()
	{
		*this = get_normalized();
	}

	inline vector2 get_normalized() const
	{
		const auto v_length = length();
		return { x / v_length, y / v_length };
	}

	inline float length() const
	{
		return sqrt(x * x + y * y);
	}

	inline std::string to_string() const
	{
		return string_format::format_string("x: %f, y: %f", x, y);
	}

	/* Operators */

	inline vector2& operator=(const vector2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	inline vector2& operator-=(const vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	inline vector2& operator+=(const vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	inline vector2& operator*=(const vector2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	inline vector2& operator/=(const vector2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	template<typename U>
	inline vector2& operator*=(const U& scalar)
	{
		*this = { x * scalar, y * scalar };
		return *this;
	}

	template<typename U>
	inline vector2& operator/=(const U& scalar)
	{
		*this = { x / scalar, y / scalar };
		return *this;
	}

	float& operator[](int idx)
	{
		assert(idx >= 0 && idx < 2 && "vector2 - index out of range");

		switch (idx)
		{
		case 0: return x;
		default: return y;
		}
	}

	const float& operator[](int idx) const
	{
		assert(idx >= 0 && idx < 2 && "vector2 - index out of range");

		switch (idx)
		{
		case 0: return x;
		default: return y;
		}
	}

};


/* Comparison operators */

inline bool operator==(const vector2& lhs, const vector2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
inline bool operator!=(const vector2& lhs, const vector2& rhs) { return !operator==(lhs, rhs); }


/* Assignment operators */

inline vector2 operator+(vector2 lhs, const vector2& rhs)
{
	lhs += rhs;
	return lhs;
}

inline vector2 operator-(vector2 lhs, const vector2& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline vector2 operator*(vector2 lhs, const vector2& rhs)
{
	lhs *= rhs;
	return lhs;
}

inline vector2 operator/(vector2 lhs, const vector2& rhs)
{
	lhs /= rhs;
	return lhs;
}

template<typename U>
inline vector2 operator*(vector2 lhs, const U& scalar)
{
	lhs *= scalar;
	return lhs;
}

template<typename U>
inline vector2 operator*(const U& scalar, vector2 rhs)
{
	rhs *= scalar;
	return rhs;
}

template<typename U>
inline vector2 operator/(vector2 lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}

inline vector2 operator-(const vector2& rhs)
{
	return rhs * -1.f;
}
