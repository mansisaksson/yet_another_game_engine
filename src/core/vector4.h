#pragma once
#include "glm/glm.hpp"

class vector4
{
private:
	vector4(const glm::vec4& t_glm_vec)
		: x(t_glm_vec.x)
		, y(t_glm_vec.y)
		, z(t_glm_vec.z)
		, w(t_glm_vec.w)
	{}

	glm::vec4 to_glm() const
	{
		return glm::vec4(x, y, z, w);
	}

	friend class matrix4x4; // Allow matrix4x4 to access our glm helper functions

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

	vector4(const float m[4])
		: x(m[0])
		, y(m[1])
		, z(m[2])
		, w(m[3])
	{}

	vector4(const vector4&o)
		: x(o.x)
		, y(o.y)
		, z(o.z)
		, w(o.w)
	{}

	static const vector4 zero;
	
	/* vector math */

	void normalize()
	{
		*this = vector4(glm::normalize(to_glm()));
	}

	vector4 get_normalized() const
	{
		return vector4(glm::normalize(to_glm()));
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
	inline vector4 operator*=(const U& scalar)
	{
		return vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	template<typename U>
	inline vector4& operator/=(const U& scalar)
	{
		return vector4(x / scalar, y / scalar, z / scalar, w / scalar);
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
inline vector4& operator/(vector4 lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}