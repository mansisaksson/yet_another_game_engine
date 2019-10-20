#pragma once
#include "platform/platformwindow.h"

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
	virtual void make_current() override;
	virtual void clear(const color& clear_color) override;
	virtual void close_window() override;
	virtual void swap_buffers() override;
	virtual std::tuple<int, int> get_window_size() const override;

	GLFWwindow* get_glfw_window() const;
};