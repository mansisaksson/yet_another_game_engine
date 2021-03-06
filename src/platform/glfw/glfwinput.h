#pragma once
#include "platform/platforminput.h"
#include <GLFW/glfw3.h>

class glfw_window;

class glfw_input : public platform_input
{
private:
	std::vector<std::weak_ptr<glfw_window>> m_windows;

public:
	// ~begin platform_input interface
	virtual void update_input() override;
	virtual std::tuple<int, int> get_cursor_location(const platform_window& t_window) override;
	// ~end platform_input interface

	void add_window(const std::weak_ptr<glfw_window> &t_window);
	void remove_window(const std::weak_ptr<glfw_window>& t_window);

private:
	static key glfw_key_to_key(int const t_glfw_key);
	static key glfw_mouse_button_to_key(int const t_glfw_button);
	static key_event glfw_key_event_to_key_event(int const t_glfw_key_event);
};