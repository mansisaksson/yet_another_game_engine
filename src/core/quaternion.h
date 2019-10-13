#pragma once
#include "glm/gtx/quaternion.hpp"

class quaternion
{
private:
	glm::quat glm_quat;

	quaternion(const glm::quat &t_glm_quat)
		: glm_quat(t_glm_quat)
		, x(glm_quat.x)
		, y(glm_quat.y)
		, z(glm_quat.z)
		, w(glm_quat.w)
	{}

public:
	float &x, y, z, w;

	quaternion()
		: glm_quat(glm::quat(1, 0, 0, 0))
		, x(glm_quat.x)
		, y(glm_quat.y)
		, z(glm_quat.z)
		, w(glm_quat.w)
	{}

	quaternion(float x, float y, float z, float w)
		: glm_quat(glm::quat(x, y, z, w))
		, x(glm_quat.x)
		, y(glm_quat.y)
		, z(glm_quat.z)
		, w(glm_quat.w)
	{}

	quaternion(const quaternion &o)
		: glm_quat(glm::quat(o.x, o.y, o.z, o.w))
		, x(glm_quat.x)
		, y(glm_quat.y)
		, z(glm_quat.z)
		, w(glm_quat.w)
	{}

	inline quaternion operator*(const quaternion& o) const
	{
		return glm_quat * o.glm_quat;
	}

	static const quaternion identity;
};