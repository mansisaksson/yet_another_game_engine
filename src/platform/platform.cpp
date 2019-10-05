#include "platform.h"

#include "platforminput.h"
#include "platformwindow.h"

#include "glfw/glfwplatform.h"

std::shared_ptr<platform> platform::get_platform()
{
	static const auto l_platform_input = std::make_shared<glfw_platform>();
	return l_platform_input;
}

std::shared_ptr<platform_window> platform::create_window()
{
	return get_platform()->create_platform_window();
}

std::shared_ptr<platform_input> platform::get_platform_input()
{
	static const auto l_platform_input = get_platform()->create_platform_input();
	return l_platform_input;
}