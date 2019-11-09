#pragma once
#include "core/core.h"
#include "collisionshape.h"

class capsule_shape : public collision_shape
{
private:
	float m_half_height;
	float m_radius;

public:

	capsule_shape()
		: collision_shape(shape_type::capsule)
	{}

	capsule_shape(float half_height, float radius)
		: m_half_height(half_height)
		, m_radius(radius)
	{}

	inline const float& get_half_height() const
	{
		return m_half_height;
	}

	inline const float& get_radius() const
	{
		return m_radius;
	}

};