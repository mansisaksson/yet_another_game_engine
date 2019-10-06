#pragma once
#include "core.h"

class transform
{
public:
	vector3 location;
	quaternion rotation;
	float scale;

	transform()
		: location(vector3::zero)
		, rotation(quaternion::identity)
		, scale(0.f)
	{}
};