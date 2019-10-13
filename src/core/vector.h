#pragma once
#include "glm/vec3.hpp"

class vector3
{
private:
	glm::vec3 glmVec;

public:
	float& x, y, z;

	vector3()
		: glmVec(0, 0, 0)
		, x(glmVec.x)
		, y(glmVec.y)
		, z(glmVec.z)
	{}

	vector3(float t_x, float t_y, float t_z)
		: glmVec(t_x, t_y, t_z)
		, x(glmVec.x)
		, y(glmVec.y)
		, z(glmVec.z)
	{}

	vector3(const vector3 &o)
		: glmVec(o.x, o.y, o.z)
		, x(glmVec.x)
		, y(glmVec.y)
		, z(glmVec.z)
	{}

	inline bool operator==(const vector3& o) const
	{
		return x == o.x && y == o.y && z == o.z;
	}

	static const vector3 zero;
	static const vector3 up;
	static const vector3 right;
	static const vector3 forward;
};