#pragma once
#include "../platformwindow.h"

struct GLFWwindow;

class glfw_window : public platform_window
{
private:
	GLFWwindow* m_window;

	glfw_window(const glfw_window&) = delete;
	glfw_window(glfw_window* const) = delete;

public:
	glfw_window();

	virtual void init(int t_width, int t_height, std::string t_title) override;
	virtual void close_window() override;

	GLFWwindow* get_glfw_window() const;
};