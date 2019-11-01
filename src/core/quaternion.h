#pragma once
#include "string.h"

class quaternion
{
public:
	float x, y, z, w;

	quaternion()
		: x(0)
		, y(0)
		, z(0)
		, w(1)
	{}

	quaternion(float x, float y, float z, float w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{}

	quaternion(const quaternion &o)
		: x(o.x)
		, y(o.y)
		, z(o.z)
		, w(o.w)
	{}

	quaternion(const vector3 &axis, float t_rad)
	{
		const float s = sin(t_rad / 2.f);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
		w = cos(t_rad / 2.f);
	}

	/* statics */

	static const quaternion identity;

	/* quaternion math */

	inline vector3 rotate_vector(vector3 v) const
	{
		const vector3 q(x, y, z);
		const vector3 t = 2.f * vector3::cross(q, v);
		const vector3 result = v + (w * t) + vector3::cross(q, t);
		return result;
	}

	inline vector3 unrotate_vector(vector3 v) const
	{
		const vector3 q(-x, -y, -z);
		const vector3 t = 2.f * vector3::cross(q, v);
		const vector3 result = v + (w * t) + vector3::cross(q, t);
		return result;
	}

	inline vector3 get_up() const
	{
		return rotate_vector(vector3::up);
	}

	inline vector3 get_right() const
	{
		return rotate_vector(vector3::right);
	}

	inline vector3 get_forward() const
	{
		return rotate_vector(vector3::forward);
	}

	inline std::string to_string() const
	{
		return yete_str::format_string("x: %f, y: %f, z: %f, w: %f", z, y, z, w);
	}

	/* Operators */

	inline quaternion& operator=(const quaternion &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	inline quaternion& operator*=(const quaternion& rhs)
	{
		quaternion result;
		result.x =  x * rhs.w + y * rhs.z - z * rhs.y + w * rhs.x;
		result.y = -x * rhs.z + y * rhs.w + z * rhs.x + w * rhs.y;
		result.z =  x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z;
		result.w = -x * rhs.x - y * rhs.y - z * rhs.z + w * rhs.w;
		*this = result;
		return *this;
	}

};

/* Comparison operators */

inline bool operator==(const quaternion &lhs, const quaternion& rhs) { return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (rhs.z == lhs.z) && (lhs.w == rhs.w); }
inline bool operator!=(const quaternion& lhs, const quaternion& rhs) { return !operator==(lhs, rhs); }


/* Assignment operators */

inline quaternion operator*(quaternion lhs, const quaternion& rhs)
{
	lhs *= rhs;
	return lhs;
}