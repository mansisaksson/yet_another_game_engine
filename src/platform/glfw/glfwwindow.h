#pragma once
#include "../platformwindow.h"

struct GLFWwindow;

class glfw_window : public platform_window
{
private:
	GLFWwindow* window;

	glfw_window() = delete;
	glfw_window(const glfw_window&) = delete;
	glfw_window(glfw_window* const) = delete;

public:
	glfw_window(int t_width, int t_height, std::string t_hint);

	virtual void close_window() override;
};