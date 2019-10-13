#pragma once
#include "glm/vec2.hpp"

class vector2
{
private:
	glm::vec2 glm_vec;

	vector2(const glm::vec2 &t_glm_vec)
		: glm_vec(t_glm_vec.x, t_glm_vec.y)
		, x(glm_vec.x)
		, y(glm_vec.y)
	{}

public:
	float& x, y;

	vector2()
		: glm_vec(0, 0)
		, x(glm_vec.x)
		, y(glm_vec.y)
	{}

	vector2(float t_x, float t_y)
		: glm_vec(t_x, t_y)
		, x(glm_vec.x)
		, y(glm_vec.y)
	{}

	vector2(const vector2& o)
		: glm_vec(o.x, o.y)
		, x(glm_vec.x)
		, y(glm_vec.y)
	{}

	inline bool operator==(const vector2& o) const
	{
		return x == o.x && y == o.y;
	}

	inline vector2 operator=(const vector2& o) const
	{
		return o;
	}

	static const vector2 zero;
};