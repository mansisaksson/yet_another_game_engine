#include "gl_viewport.h"
#include <GL/glew.h>

std::tuple<int, int> yete_to_gl_coordinates(int x, int y, int width, int height, int window_width, int window_height)
{
	return { x, window_height - y - height };
}

void gl_viewport::make_current(int x, int y, int width, int height, int window_width, int widow_height)
{
	const auto gl_coordinates = yete_to_gl_coordinates(x, y, width, height, window_width, widow_height);
	glViewport(std::get<0>(gl_coordinates), std::get<1>(gl_coordinates), width, height);
}