#pragma once
#include "core/core.h"
#include "collisionshape.h"

class sphere_shape : public collision_shape
{
private:
	float m_radius;

public:

	sphere_shape()
		: collision_shape(shape_type::box)
	{}

	sphere_shape(float radius)
		: m_radius(radius)
	{}

	inline const float& get_radius() const
	{
		return m_radius;
	}
};