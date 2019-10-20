#include "glfwwindow.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "platform/platform.h"
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
		log::error("glfw_window", "NO WINDOW");
		return;
	}

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	static bool m_glew_needs_init = true;
	if (m_glew_needs_init)
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			log::error("glfw_platform", "Error: %s", glewGetErrorString(err));
			exit(EXIT_FAILURE);
		}
		m_glew_needs_init = false;
	}


	const auto platform_input = std::static_pointer_cast<glfw_input>(platform::get_platform_input());
	platform_input->add_window(std::static_pointer_cast<glfw_window>(shared_from_this()));
}

void glfw_window::close_window()
{
	on_window_closed.broadcast(*this);
	//std::static_pointer_cast<glfw_input>(platform::get_platform_input())->remove_window(this);
	glfwDestroyWindow(m_window);
	m_window = nullptr;
}

void glfw_window::make_current()
{
	glfwMakeContextCurrent(m_window);
}

void glfw_window::clear(const color& clear_color) 
{
	glClearColor(
		(float)clear_color.r / 255.f,
		(float)clear_color.g / 255.f, 
		(float)clear_color.b / 255.f, 
		(float)clear_color.a / 255.f
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void glfw_window::swap_buffers()
{
	glfwSwapBuffers(m_window);
}

std::tuple<int, int> glfw_window::get_window_size() const
{
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	return { width, height };
}

GLFWwindow* glfw_window::get_glfw_window() const
{
	return m_window;
}