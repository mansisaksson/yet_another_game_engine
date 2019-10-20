#pragma once
#include "rendercore/viewport.h"

class gl_viewport : public viewport
{
public:
	gl_viewport(const vector3& t_location, float t_fov, float t_aspect, float t_z_near, float t_z_far);

	virtual void make_current(int x, int y, int width, int height) override;
};