#pragma once
#include "../platforminput.h"
#include <GLFW/glfw3.h>

class glfw_input : public platform_input
{
private:
	std::vector<GLFWwindow*> m_window;

public:
	// ~begin platform_input interface
	virtual void update_input() override;
	virtual std::tuple<int, int> get_cursor_location() override;
	// ~end platform_input interface

	void add_window(GLFWwindow* const t_window);
	void remove_window(GLFWwindow* const t_window);

private:
	static key glfw_key_to_key(int const t_glfw_key);
	static key glfw_mouse_button_to_key(int const t_glfw_button);
	static key_event glfw_key_event_to_key_event(int const t_glfw_key_event);
};