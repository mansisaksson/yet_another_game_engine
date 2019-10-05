#include "glfwwindow.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include "../platform.h"
#include "glfwinput.h"

glfw_window::glfw_window()
	: m_window(nullptr)
{

}

void glfw_window::init(int t_width, int t_height, std::string t_title)
{
	m_window = glfwCreateWindow(t_width, t_height, t_title.c_str(), NULL, NULL);
	if (!m_window)
	{
		std::cout << "NO WINDOW!\n";
		return;
	}

	const auto platform_input = std::static_pointer_cast<glfw_input>(platform::get_platform_input());
	platform_input->add_window(std::static_pointer_cast<glfw_window>(shared_from_this()));
}

void glfw_window::close_window()
{
	//std::static_pointer_cast<glfw_input>(platform::get_platform_input())->remove_window(this);
	glfwDestroyWindow(m_window);
	m_window = nullptr;
}

GLFWwindow* glfw_window::get_glfw_window() const
{
	return m_window;
}