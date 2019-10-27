#pragma once
#include "string.h"

class vector4
{
public:
	float x, y, z, w;

	vector4()
		: x(0)
		, y(0)
		, z(0)
		, w(0)
	{}

	vector4(float t_x, float t_y, float t_z, float t_w)
		: x(t_x)
		, y(t_y)
		, z(t_z)
		, w(t_w)
	{}

	vector4(const float v[4])
		: x(v[0])
		, y(v[1])
		, z(v[2])
		, w(v[3])
	{}

	vector4(const vector4&o)
		: x(o.x)
		, y(o.y)
		, z(o.z)
		, w(o.w)
	{}

	static const vector4 zero;
	
	/* vector math */

	inline void normalize()
	{
		*this = get_normalized();
	}

	inline vector4 get_normalized() const
	{
		const auto v_length = length();
		return { x / v_length, y / v_length, z / v_length, w / v_length };
	}

	inline float length() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

	static float dot(const vector4& v1, const vector4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	std::string to_string() const
	{
		return string_format::format_string("x: %f, y: %f, z: %f, w: %f", x, y, z, w);
	}

	/* Operators */

	inline vector4& operator=(const vector4&rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	inline vector4& operator-=(const vector4& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	inline vector4& operator+=(const vector4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w -= rhs.w;
		return *this;
	}

	inline vector4& operator*=(const vector4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	inline vector4& operator/=(const vector4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	template<typename U>
	inline vector4 &operator*=(const U& scalar)
	{
		*this = { x * scalar, y * scalar, z * scalar, w * scalar };
		return *this;
	}

	template<typename U>
	inline vector4& operator/=(const U& scalar)
	{
		*this = { x / scalar, y / scalar, z / scalar, w / scalar };
		return *this;
	}

	float& operator[](int idx)
	{
		assert(idx >= 0 && idx < 4 && "vector4 - index out of range");

		switch (idx)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return w;
		}
		return x;
	}

	const float& operator[](int idx) const
	{
		assert(idx >= 0 && idx < 4 && "vector4 - index out of range");

		switch (idx)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return w;
		}
	}
};


/* Comparison operators */

inline bool operator==(const vector4& lhs, const vector4& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w; }
inline bool operator!=(const vector4& lhs, const vector4& rhs) { return !operator==(lhs, rhs); }


/* Assignment operators */

inline vector4 operator+(vector4 lhs, const vector4& rhs)
{
	lhs += rhs;
	return lhs;
}

inline vector4 operator-(vector4 lhs, const vector4& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline vector4 operator*(vector4 lhs, const vector4& rhs)
{
	lhs *= rhs;
	return lhs;
}

inline vector4 operator/(vector4 lhs, const vector4& rhs)
{
	lhs /= rhs;
	return lhs;
}

template<typename U>
inline vector4 operator*(vector4 lhs, const U& scalar)
{
	lhs *= scalar;
	return lhs;
}

template<typename U>
inline vector4 operator*(const U& scalar, vector4 rhs)
{
	rhs *= scalar;
	return rhs;
}

template<typename U>
inline vector4 operator/(vector4 lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}