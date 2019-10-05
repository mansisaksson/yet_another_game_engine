#include "glfwinput.h"
#include <GLFW/glfw3.h>
#include <algorithm>

#include "../platform.h"
#include "glfwwindow.h"

void glfw_input::update_input()
{
	glfwPollEvents();

	std::vector<std::weak_ptr<glfw_window>> windows_to_remove;

	for (const auto & window : m_windows)
	{
		if (window.expired() || glfwWindowShouldClose(window.lock()->get_glfw_window()))
			windows_to_remove.push_back(window);
	}

	for (const auto& window : windows_to_remove)
	{
		if (!window.expired())
		{
			remove_window(window.lock());
			window.lock()->close_window();
		}
	}
}

std::tuple<int, int> glfw_input::get_cursor_location()
{
	return { 0, 0 };
}

void glfw_input::add_window(const std::weak_ptr<glfw_window> &t_window)
{
	m_windows.push_back(t_window);

	struct local
	{
		static void key_callback(GLFWwindow* t_window, int t_key, int t_scancode, int t_action, int t_mods)
		{
			input_event input_event =
			{
				glfw_key_to_key(t_key),
				glfw_key_event_to_key_event(t_action)
			};

			std::static_pointer_cast<glfw_input>(platform::get_platform_input())->generate_input_event(input_event);
		}

		static void mouse_button_callback(GLFWwindow* const window, int t_button, int t_action, int t_mods)
		{
			input_event input_event =
			{
				glfw_mouse_button_to_key(t_button),
				glfw_key_event_to_key_event(t_action)
			};

			std::static_pointer_cast<glfw_input>(platform::get_platform_input())->generate_input_event(input_event);
		}
	};

	const auto glfw_window = t_window.lock()->get_glfw_window();
	glfwSetKeyCallback(glfw_window, local::key_callback);
	glfwSetMouseButtonCallback(glfw_window, local::mouse_button_callback);
}

void glfw_input::remove_window(const std::weak_ptr<glfw_window>& t_window)
{
	const auto l_glfw_window = t_window.lock()->get_glfw_window();

	glfwSetKeyCallback(l_glfw_window, nullptr);
	glfwSetMouseButtonCallback(l_glfw_window, nullptr);

	m_windows.erase(
		std::remove_if(m_windows.begin(), m_windows.end(), [&](const auto& glfw_window) { return glfw_window.lock() == t_window.lock(); }),
		m_windows.end()
	);
}

key glfw_input::glfw_key_to_key(int const t_glfw_key)
{
	switch (t_glfw_key)
	{
	case GLFW_KEY_SPACE:
		return key::e_space;
	case GLFW_KEY_APOSTROPHE:
		return key::e_apostrophe;
	case GLFW_KEY_COMMA:
		return key::e_comma;
	case GLFW_KEY_MINUS:
		return key::e_minus;
	case GLFW_KEY_PERIOD:
		return key::e_period;
	case GLFW_KEY_SLASH:
		return key::e_slash;
	case GLFW_KEY_0:
		return key::e_0;
	case GLFW_KEY_1:
		return key::e_1;
	case GLFW_KEY_2:
		return key::e_2;
	case GLFW_KEY_3:
		return key::e_3;
	case GLFW_KEY_4:
		return key::e_4;
	case GLFW_KEY_5:
		return key::e_5;
	case GLFW_KEY_6:
		return key::e_6;
	case GLFW_KEY_7:
		return key::e_7;
	case GLFW_KEY_8:
		return key::e_8;
	case GLFW_KEY_9:
		return key::e_9;
	case GLFW_KEY_SEMICOLON:
		return key::e_semicolon;
	case GLFW_KEY_EQUAL:
		return key::e_equal;
	case GLFW_KEY_A:
		return key::e_a;
	case GLFW_KEY_B:
		return key::e_b;
	case GLFW_KEY_C:
		return key::e_c;
	case GLFW_KEY_D:
		return key::e_d;
	case GLFW_KEY_E:
		return key::e_e;
	case GLFW_KEY_F:
		return key::e_f;
	case GLFW_KEY_G:
		return key::e_g;
	case GLFW_KEY_H:
		return key::e_h;
	case GLFW_KEY_I:
		return key::e_i;
	case GLFW_KEY_J:
		return key::e_j;
	case GLFW_KEY_K:
		return key::e_k;
	case GLFW_KEY_L:
		return key::e_l;
	case GLFW_KEY_M:
		return key::e_m;
	case GLFW_KEY_N:
		return key::e_n;
	case GLFW_KEY_O:
		return key::e_o;
	case GLFW_KEY_P:
		return key::e_p;
	case GLFW_KEY_Q:
		return key::e_q;
	case GLFW_KEY_R:
		return key::e_r;
	case GLFW_KEY_S:
		return key::e_s;
	case GLFW_KEY_T:
		return key::e_t;
	case GLFW_KEY_U:
		return key::e_u;
	case GLFW_KEY_V:
		return key::e_v;
	case GLFW_KEY_W:
		return key::e_w;
	case GLFW_KEY_X:
		return key::e_x;
	case GLFW_KEY_Y:
		return key::e_y;
	case GLFW_KEY_Z:
		return key::e_z;
	case GLFW_KEY_LEFT_BRACKET:
		return key::e_left_bracket;
	case GLFW_KEY_BACKSLASH:
		return key::e_backslash;
	case GLFW_KEY_RIGHT_BRACKET:
		return key::e_right_bracket;
	case GLFW_KEY_GRAVE_ACCENT:
		return key::e_grave_accent;
	case GLFW_KEY_WORLD_1:
		return key::e_world_1;
	case GLFW_KEY_WORLD_2:
		return key::e_world_2;
	case GLFW_KEY_ESCAPE:
		return key::e_escape;
	case GLFW_KEY_ENTER:
		return key::e_enter;
	case GLFW_KEY_TAB:
		return key::e_tab;
	case GLFW_KEY_BACKSPACE:
		return key::e_backspace;
	case GLFW_KEY_INSERT:
		return key::e_insert;
	case GLFW_KEY_DELETE:
		return key::e_delete;
	case GLFW_KEY_RIGHT:
		return key::e_right;
	case GLFW_KEY_LEFT:
		return key::e_left;
	case GLFW_KEY_DOWN:
		return key::e_down;
	case GLFW_KEY_UP:
		return key::e_up;
	case GLFW_KEY_PAGE_UP:
		return key::e_page_up;
	case GLFW_KEY_PAGE_DOWN:
		return key::e_page_down;
	case GLFW_KEY_HOME:
		return key::e_home;
	case GLFW_KEY_END:
		return key::e_end;
	case GLFW_KEY_CAPS_LOCK:
		return key::e_caps_lock;
	case GLFW_KEY_SCROLL_LOCK:
		return key::e_scroll_lock;
	case GLFW_KEY_NUM_LOCK:
		return key::e_num_lock;
	case GLFW_KEY_PRINT_SCREEN:
		return key::e_print_screen;
	case GLFW_KEY_PAUSE:
		return key::e_pause;
	case GLFW_KEY_F1:
		return key::e_f1;
	case GLFW_KEY_F2:
		return key::e_f2;
	case GLFW_KEY_F3:
		return key::e_f3;
	case GLFW_KEY_F4:
		return key::e_f4;
	case GLFW_KEY_F5:
		return key::e_f5;
	case GLFW_KEY_F6:
		return key::e_f6;
	case GLFW_KEY_F7:
		return key::e_f7;
	case GLFW_KEY_F8:
		return key::e_f8;
	case GLFW_KEY_F9:
		return key::e_f9;
	case GLFW_KEY_F10:
		return key::e_f10;
	case GLFW_KEY_F11:
		return key::e_f11;
	case GLFW_KEY_F12:
		return key::e_f12;
	case GLFW_KEY_F13:
		return key::e_f13;
	case GLFW_KEY_F14:
		return key::e_f14;
	case GLFW_KEY_F15:
		return key::e_f15;
	case GLFW_KEY_F16:
		return key::e_f16;
	case GLFW_KEY_F17:
		return key::e_f17;
	case GLFW_KEY_F18:
		return key::e_f18;
	case GLFW_KEY_F19:
		return key::e_f19;
	case GLFW_KEY_F20:
		return key::e_f20;
	case GLFW_KEY_F21:
		return key::e_f21;
	case GLFW_KEY_F22:
		return key::e_f22;
	case GLFW_KEY_F23:
		return key::e_f23;
	case GLFW_KEY_F24:
		return key::e_f24;
	case GLFW_KEY_F25:
		return key::e_f25;
	case GLFW_KEY_KP_0:
		return key::e_kp_0;
	case GLFW_KEY_KP_1:
		return key::e_kp_1;
	case GLFW_KEY_KP_2:
		return key::e_kp_2;
	case GLFW_KEY_KP_3:
		return key::e_kp_3;
	case GLFW_KEY_KP_4:
		return key::e_kp_4;
	case GLFW_KEY_KP_5:
		return key::e_kp_5;
	case GLFW_KEY_KP_6:
		return key::e_kp_6;
	case GLFW_KEY_KP_7:
		return key::e_kp_7;
	case GLFW_KEY_KP_8:
		return key::e_kp_8;
	case GLFW_KEY_KP_9:
		return key::e_kp_9;
	case GLFW_KEY_KP_DECIMAL:
		return key::e_kp_decimal;
	case GLFW_KEY_KP_DIVIDE:
		return key::e_kp_divide;
	case GLFW_KEY_KP_MULTIPLY:
		return key::e_kp_multiply;
	case GLFW_KEY_KP_SUBTRACT:
		return key::e_kp_subtract;
	case GLFW_KEY_KP_ADD:
		return key::e_kp_add;
	case GLFW_KEY_KP_ENTER:
		return key::e_kp_enter;
	case GLFW_KEY_KP_EQUAL:
		return key::e_kp_equal;
	case GLFW_KEY_LEFT_SHIFT:
		return key::e_left_shift;
	case GLFW_KEY_LEFT_CONTROL:
		return key::e_left_control;
	case GLFW_KEY_LEFT_ALT:
		return key::e_left_alt;
	case GLFW_KEY_LEFT_SUPER:
		return key::e_left_super;
	case GLFW_KEY_RIGHT_SHIFT:
		return key::e_right_shift;
	case GLFW_KEY_RIGHT_CONTROL:
		return key::e_right_control;
	case GLFW_KEY_RIGHT_ALT:
		return key::e_right_alt;
	case GLFW_KEY_RIGHT_SUPER:
		return key::e_right_super;
	case GLFW_KEY_MENU:
		return key::e_menu;

	default:
		return key::e_unknown;
	}
}

key glfw_input::glfw_mouse_button_to_key(int const t_glfw_button)
{
	switch (t_glfw_button)
	{
		case GLFW_MOUSE_BUTTON_1:
			return key::e_mouse_button_1;
		case GLFW_MOUSE_BUTTON_2:
			return key::e_mouse_button_2;
		case GLFW_MOUSE_BUTTON_3:
			return key::e_mouse_button_middle;
		case GLFW_MOUSE_BUTTON_4:
			return key::e_mouse_button_4;
		case GLFW_MOUSE_BUTTON_5:
			return key::e_mouse_button_5;
		case GLFW_MOUSE_BUTTON_6:
			return key::e_mouse_button_6;
		case GLFW_MOUSE_BUTTON_7:
			return key::e_mouse_button_7;
		case GLFW_MOUSE_BUTTON_8:
			return key::e_mouse_button_8;
	}

	return key::e_mouse_button_1;
}

key_event glfw_input::glfw_key_event_to_key_event(int t_glfw_key_event)
{
	switch (t_glfw_key_event)
	{
	case GLFW_PRESS:
		return key_event::e_pressed;
	case GLFW_RELEASE:
		return key_event::e_released;
	case GLFW_REPEAT:
		return key_event::e_repeat;
	}

	return key_event::e_released;
}