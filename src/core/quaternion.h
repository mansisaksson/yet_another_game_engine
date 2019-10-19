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

	inline quaternion& operator*(const quaternion& o)
	{
		*this = quaternion(to_glm() * o.to_glm());
		return *this;
	}

	inline quaternion& operator=(const quaternion& o)
	{
		x = o.x;
		y = o.y;
		z = o.z;
		w = o.w;
		return *this;
	}

	static const quaternion identity;
};