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

	transform(const vector3& t_location, const quaternion &rotation, float scale)
		: location(t_location)
		, rotation(rotation)
		, scale(scale)
	{}

	static const transform identity;

	inline matrix4x4 to_matrix() const
	{
		matrix4x4 out_matrix = matrix4x4::identity;

		

		return out_matrix;
	}

};