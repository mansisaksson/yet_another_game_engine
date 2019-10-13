#pragma once
#include "glm/vec3.hpp"

class vector3
{
private:
	glm::vec3 glm_vec;

	vector3(const glm::vec3& t_glm_vec)
		: glm_vec(t_glm_vec)
		, x(glm_vec.x)
		, y(glm_vec.y)
		, z(glm_vec.z)
	{}

public:
	float& x, y, z;

	vector3()
		: glm_vec(0, 0, 0)
		, x(glm_vec.x)
		, y(glm_vec.y)
		, z(glm_vec.z)
	{}

	vector3(float t_x, float t_y, float t_z)
		: glm_vec(t_x, t_y, t_z)
		, x(glm_vec.x)
		, y(glm_vec.y)
		, z(glm_vec.z)
	{}

	vector3(const vector3 &o)
		: glm_vec(o.x, o.y, o.z)
		, x(glm_vec.x)
		, y(glm_vec.y)
		, z(glm_vec.z)
	{}

	inline bool operator==(const vector3& o) const
	{
		return x == o.x && y == o.y && z == o.z;
	}

	inline vector3 operator=(const vector3& o) const
	{
		return o;
	}

	static const vector3 zero;
	static const vector3 up;
	static const vector3 right;
	static const vector3 forward;
};