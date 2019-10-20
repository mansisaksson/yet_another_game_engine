#pragma once
#include "core/core.h"

class viewport : public std::enable_shared_from_this<viewport>
{
public:
	matrix4x4 perspective_matrix;
	vector3 location;
	quaternion rotation;

protected:
	viewport() = default;

public:

	static std::shared_ptr<viewport> create_viewport();

	void set_view_location(const vector3& t_location);
	void set_view_rotation(const quaternion& t_rotation);
	void set_view_perspective(float t_fov, float t_aspect, float t_z_near, float t_z_far);

	matrix4x4 get_view_projection() const;

	virtual void make_current(int x, int y, int width, int height, int window_width, int widow_height) = 0;

};