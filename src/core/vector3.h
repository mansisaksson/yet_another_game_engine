#pragma once
#include "glm/glm.hpp"
#include "string.h"

class vector3
{
public:
	float x, y, z;

	vector3()
		: x(0)
		, y(0)
		, z(0)
	{}

	vector3(float t_x, float t_y, float t_z)
		: x(t_x)
		, y(t_y)
		, z(t_z)
	{}

	vector3(const float v[3])
		: x(v[0])
		, y(v[1])
		, z(v[2])
	{}

	vector3(const vector3 &o)
		: x(o.x)
		, y(o.y)
		, z(o.z)
	{}

	/* statics */

	static const vector3 zero;
	static const vector3 one;
	static const vector3 up;
	static const vector3 right;
	static const vector3 forward;

	/* vector math */

	void normalize()
	{
		*this = get_normalized();
	}

	inline vector3 get_normalized() const
	{
		const auto v_length = length();
		return { x / v_length, y / v_length, z / v_length };
	}

	inline float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	static vector3 cross(const vector3& v1, const vector3& v2)
	{
		return {
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		};
	}

	static float dot(const vector3& v1, const vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	inline std::string to_string() const
	{
		return string_format::format_string("x: %f, y: %f, z: %f", x, y, z);
	}

	/* Operators */

	inline vector3& operator=(const vector3 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	inline vector3& operator-=(const vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	inline vector3& operator+=(const vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	inline vector3& operator*=(const vector3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	inline vector3& operator/=(const vector3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	template<typename U>
	inline vector3& operator*=(const U& scalar)
	{
		*this = { x * scalar, y * scalar, z * scalar };
		return *this;
	}

	template<typename U>
	inline vector3& operator/=(const U& scalar)
	{
		*this = { x / scalar, y / scalar, z / scalar };
		return *this;
	}

	float& operator[](int idx)
	{
		assert(idx >= 0 && idx < 3 && "vector3 - index out of range");

		switch (idx)
		{
		case 0: return x;
		case 1: return y;
		default: return z;
		}
	}

	const float& operator[](int idx) const
	{
		assert(idx >= 0 && idx < 3 && "vector3 - index out of range");

		switch (idx)
		{
		case 0: return x;
		case 1: return y;
		default: return z;
		}
	}
};


/* Comparison operators */

inline bool operator==(const vector3& lhs, const vector3& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
inline bool operator!=(const vector3& lhs, const vector3& rhs) { return !operator==(lhs, rhs); }


/* Assignment operators */

inline vector3 operator+(vector3 lhs, const vector3& rhs)
{
	lhs += rhs;
	return lhs;
}

inline vector3 operator-(vector3 lhs, const vector3& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline vector3 operator*(vector3 lhs, const vector3& rhs)
{
	lhs *= rhs;
	return lhs;
}

inline vector3 operator/(vector3 lhs, const vector3& rhs)
{
	lhs /= rhs;
	return lhs;
}

template<typename U>
inline vector3 operator*(vector3 lhs, const U& scalar)
{
	lhs *= scalar;
	return lhs;
}

template<typename U>
inline vector3 operator*(const U& scalar, vector3 rhs)
{
	rhs *= scalar;
	return rhs;
}

template<typename U>
inline vector3 operator/(vector3 lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}

inline vector3 operator-(const vector3& rhs)
{
	return rhs * -1.f;
}