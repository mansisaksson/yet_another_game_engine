
#include <GL/glew.h>

#include "glfwplatform.h"
#include "glfwinput.h"
#include "glfwwindow.h"

glfw_platform::glfw_platform()
{
	glfwInit();
	if (!glfwInit())
	{
		log::error("glfw_platform", "Failed to initialize glfw");
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

glfw_platform::~glfw_platform()
{
	glfwTerminate();
}

std::shared_ptr<platform_window> glfw_platform::create_platform_window()
{
	return std::make_shared<glfw_window>();
}

std::shared_ptr<platform_input> glfw_platform::create_platform_input()
{
	return std::make_shared<glfw_input>();
}

float glfw_platform::get_platform_time() const
{
	return (float)glfwGetTime();
}