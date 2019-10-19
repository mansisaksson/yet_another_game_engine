#pragma once
#include "glm/glm.hpp"

class vector3
{
private:
	vector3(const glm::vec3& t_glm_vec)
		: x(t_glm_vec.x)
		, y(t_glm_vec.y)
		, z(t_glm_vec.z)
	{}

	glm::vec3 to_glm() const
	{
		return glm::vec3(x, y, z);
	}

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

	vector3(const vector3 &o)
		: x(o.x)
		, y(o.y)
		, z(o.z)
	{}

	/* statics */

	static const vector3 zero;
	static const vector3 up;
	static const vector3 right;
	static const vector3 forward;

	/* vector math */

	void normalize()
	{
		*this = glm::normalize(to_glm());
	}

	vector3 get_normalized() const
	{
		return glm::normalize(to_glm());
	}

	static vector3 cross(const vector3& v1, const vector3& v2)
	{
		return glm::cross(v1.to_glm(), v2.to_glm());
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
	inline vector3 operator*=(const U& scalar)
	{
		return vector3(x * scalar, y * scalar, z * scalar);
	}

	template<typename U>
	inline vector3& operator/=(const U& scalar)
	{
		return vector3(x / scalar, y / scalar, z / scalar);
	}

	float& operator[](int idx)
	{
		assert(idx > 0 && idx < 3 && "vector3 - index out of range");

		switch (idx)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
	}

	const float& operator[](int idx) const
	{
		assert(idx > 0 && idx < 3 && "vector3 - index out of range");

		switch (idx)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
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
inline vector3& operator/(vector3 lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}