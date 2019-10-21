#pragma once
#include "glm/gtx/quaternion.hpp"

class quaternion
{
private:
	quaternion(const glm::quat &t_glm_quat)
		: x(t_glm_quat.x)
		, y(t_glm_quat.y)
		, z(t_glm_quat.z)
		, w(t_glm_quat.w)
	{}

	glm::quat to_glm() const
	{
		return glm::quat(x, y, z, w);
	}

public:
	float x, y, z, w;

	quaternion()
		: x(0)
		, y(0)
		, z(0)
		, w(0)
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

	vector3 get_up() const
	{
		return rotate_vector(vector3::up);
	}

	vector3 get_right() const
	{
		return rotate_vector(vector3::right);
	}

	vector3 get_forward() const
	{
		return rotate_vector(vector3::forward);
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

	inline quaternion& operator*=(const quaternion& o)
	{
		*this = quaternion(to_glm() * o.to_glm());
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