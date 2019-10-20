#pragma once
#include "core/core.h"
#include <glm/gtx/transform.hpp>

class viewport
{
public:
	matrix4x4 perspective_matrix;
	vector3 location;
	quaternion rotation;

public:
	viewport(const vector3& t_location, float t_fov, float t_aspect, float t_z_near, float t_z_far)
	{
		update_viewport(t_location, t_fov, t_aspect, t_z_near, t_z_far);
	}

	virtual void update_viewport(const vector3& t_location, float t_fov, float t_aspect, float t_z_near, float t_z_far)
	{
		location = t_location;
		perspective_matrix = matrix4x4::perspective(t_fov, t_aspect, t_z_near, t_z_far);
	}

	virtual matrix4x4 get_view_projection() const
	{
		return perspective_matrix * matrix4x4::look_at(location, location + rotation.get_forward(), rotation.get_up());
	}

	virtual void make_current(int x, int y, int width, int height) = 0;

};