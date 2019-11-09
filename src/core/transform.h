#pragma once
#include "vector3.h"
#include "quaternion.h"
#include "matrix4x4.h"

class transform
{
public:
	vector3 location;
	quaternion rotation;
	float scale;

	transform()
		: location(vector3::zero)
		, rotation(quaternion::identity)
		, scale(1.f)
	{}

	transform(const vector3& t_location, const quaternion &rotation = quaternion::identity, float scale = 1.f)
		: location(t_location)
		, rotation(rotation)
		, scale(scale)
	{}

	static const transform identity;

	matrix4x4 to_matrix() const
	{
		const matrix4x4 translation_matrix =
		{
			{ 1, 0, 0, location.x },
			{ 0, 1, 0, location.y },
			{ 0, 0, 1, location.z },
			{ 0, 0, 0, 1 },
		};

		const auto forward = rotation.get_forward();
		const auto right = rotation.get_right();
		const auto up = rotation.get_up();

		const matrix4x4 rotation_matrix =
		{
			{ forward.x,	forward.y,	forward.z,	0 },
			{ right.x,		right.y,	right.z,	0 },
			{ up.x,			up.y,		up.z,		0 },
			{ 0,			0,			0,			1 }
		};

		const matrix4x4 scale_matrix =
		{
			{ scale,	0,		0,		0 },
			{ 0,		scale,	0,		0 },
			{ 0,		0,		scale,	0 },
			{ 0,		0,		0,		1 },
		};

		return translation_matrix * rotation_matrix * scale_matrix;
	}

};