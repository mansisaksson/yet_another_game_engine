#pragma once
#include "core/core.h"

struct vertex
{
	vector3 position;
	vector2 tex_coord;
	vector3 normal;

	vertex(const vector3& t_position, const vector2& t_tex_coord, const vector3& t_normal = vector3(0, 0, 0))
		: position(t_position)
		, tex_coord(t_tex_coord)
		, normal(t_normal)
	{}
};