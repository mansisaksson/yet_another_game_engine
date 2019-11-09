#pragma once
#include "core/core.h"
#include "collisionshape.h"

class box_shape : public collision_shape
{
private:
	vector3 m_half_extent;

public:

	box_shape()
		: collision_shape(shape_type::box)
	{}

	box_shape(vector3 half_extent)
		: collision_shape(shape_type::box)
		, m_half_extent(half_extent)
	{}

	const vector3 &get_box_half_extent() const
	{
		return m_half_extent;
	}
};