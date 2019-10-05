#include "glfwwindow.h"

#include <GLFW/glfw3.h>
#include <iostream>

glfw_window::glfw_window(int t_width, int t_height, std::string t_hint)
	: window(nullptr)
{
	window = glfwCreateWindow(t_width, t_height, t_hint.c_str(), NULL, NULL);
	if (!window)
	{
		std::cout << "NO WINDOW!\n";
	}
}

void glfw_window::close_window()
{
	glfwDestroyWindow(window);
}