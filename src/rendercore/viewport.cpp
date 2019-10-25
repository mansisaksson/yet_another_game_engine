#pragma once
#include "viewport.h"
#include "opengl/gl_viewport.h"

std::shared_ptr<viewport> viewport::create_viewport()
{
	return std::make_shared<gl_viewport>();
}

void viewport::set_view_location(const vector3& t_location)
{
	location = t_location;
}

void viewport::set_view_rotation(const quaternion& t_rotation)
{
	rotation = t_rotation;
}

void viewport::set_view_perspective(float t_fov, float t_aspect, float t_z_near, float t_z_far)
{
	perspective_matrix = matrix4x4::perspective(t_fov, t_aspect, t_z_near, t_z_far);
}

matrix4x4 viewport::get_view_projection() const
{
	return perspective_matrix * matrix4x4::look_at(location, location + rotation.get_forward(), rotation.get_up());
}