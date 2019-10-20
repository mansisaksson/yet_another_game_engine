#include "gl_viewport.h"
#include <GL/glew.h>

gl_viewport::gl_viewport(const vector3& t_location, float t_fov, float t_aspect, float t_z_near, float t_z_far)
	: viewport(t_location, t_fov, t_aspect, t_z_near, t_z_far)
{
}

void gl_viewport::make_current(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}