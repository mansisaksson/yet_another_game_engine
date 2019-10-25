#include "glfwinput.h"
#include <GLFW/glfw3.h>
#include <algorithm>

#include "platform/platform.h"
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
		return key::space;
	case GLFW_KEY_APOSTROPHE:
		return key::apostrophe;
	case GLFW_KEY_COMMA:
		return key::comma;
	case GLFW_KEY_MINUS:
		return key::minus;
	case GLFW_KEY_PERIOD:
		return key::period;
	case GLFW_KEY_SLASH:
		return key::slash;
	case GLFW_KEY_0:
		return key::zero;
	case GLFW_KEY_1:
		return key::one;
	case GLFW_KEY_2:
		return key::two;
	case GLFW_KEY_3:
		return key::three;
	case GLFW_KEY_4:
		return key::four;
	case GLFW_KEY_5:
		return key::five;
	case GLFW_KEY_6:
		return key::six;
	case GLFW_KEY_7:
		return key::seven;
	case GLFW_KEY_8:
		return key::eight;
	case GLFW_KEY_9:
		return key::nine;
	case GLFW_KEY_SEMICOLON:
		return key::semicolon;
	case GLFW_KEY_EQUAL:
		return key::equal;
	case GLFW_KEY_A:
		return key::a;
	case GLFW_KEY_B:
		return key::b;
	case GLFW_KEY_C:
		return key::c;
	case GLFW_KEY_D:
		return key::d;
	case GLFW_KEY_E:
		return key::e;
	case GLFW_KEY_F:
		return key::f;
	case GLFW_KEY_G:
		return key::g;
	case GLFW_KEY_H:
		return key::h;
	case GLFW_KEY_I:
		return key::i;
	case GLFW_KEY_J:
		return key::j;
	case GLFW_KEY_K:
		return key::k;
	case GLFW_KEY_L:
		return key::l;
	case GLFW_KEY_M:
		return key::m;
	case GLFW_KEY_N:
		return key::n;
	case GLFW_KEY_O:
		return key::o;
	case GLFW_KEY_P:
		return key::p;
	case GLFW_KEY_Q:
		return key::q;
	case GLFW_KEY_R:
		return key::r;
	case GLFW_KEY_S:
		return key::s;
	case GLFW_KEY_T:
		return key::t;
	case GLFW_KEY_U:
		return key::u;
	case GLFW_KEY_V:
		return key::v;
	case GLFW_KEY_W:
		return key::w;
	case GLFW_KEY_X:
		return key::x;
	case GLFW_KEY_Y:
		return key::y;
	case GLFW_KEY_Z:
		return key::z;
	case GLFW_KEY_LEFT_BRACKET:
		return key::left_bracket;
	case GLFW_KEY_BACKSLASH:
		return key::backslash;
	case GLFW_KEY_RIGHT_BRACKET:
		return key::right_bracket;
	case GLFW_KEY_GRAVE_ACCENT:
		return key::grave_accent;
	case GLFW_KEY_WORLD_1:
		return key::world_1;
	case GLFW_KEY_WORLD_2:
		return key::world_2;
	case GLFW_KEY_ESCAPE:
		return key::escape;
	case GLFW_KEY_ENTER:
		return key::enter;
	case GLFW_KEY_TAB:
		return key::tab;
	case GLFW_KEY_BACKSPACE:
		return key::backspace;
	case GLFW_KEY_INSERT:
		return key::insert;
	case GLFW_KEY_DELETE:
		return key::del;
	case GLFW_KEY_RIGHT:
		return key::right;
	case GLFW_KEY_LEFT:
		return key::left;
	case GLFW_KEY_DOWN:
		return key::down;
	case GLFW_KEY_UP:
		return key::up;
	case GLFW_KEY_PAGE_UP:
		return key::page_up;
	case GLFW_KEY_PAGE_DOWN:
		return key::page_down;
	case GLFW_KEY_HOME:
		return key::home;
	case GLFW_KEY_END:
		return key::end;
	case GLFW_KEY_CAPS_LOCK:
		return key::caps_lock;
	case GLFW_KEY_SCROLL_LOCK:
		return key::scroll_lock;
	case GLFW_KEY_NUM_LOCK:
		return key::num_lock;
	case GLFW_KEY_PRINT_SCREEN:
		return key::print_screen;
	case GLFW_KEY_PAUSE:
		return key::pause;
	case GLFW_KEY_F1:
		return key::f1;
	case GLFW_KEY_F2:
		return key::f2;
	case GLFW_KEY_F3:
		return key::f3;
	case GLFW_KEY_F4:
		return key::f4;
	case GLFW_KEY_F5:
		return key::f5;
	case GLFW_KEY_F6:
		return key::f6;
	case GLFW_KEY_F7:
		return key::f7;
	case GLFW_KEY_F8:
		return key::f8;
	case GLFW_KEY_F9:
		return key::f9;
	case GLFW_KEY_F10:
		return key::f10;
	case GLFW_KEY_F11:
		return key::f11;
	case GLFW_KEY_F12:
		return key::f12;
	case GLFW_KEY_F13:
		return key::f13;
	case GLFW_KEY_F14:
		return key::f14;
	case GLFW_KEY_F15:
		return key::f15;
	case GLFW_KEY_F16:
		return key::f16;
	case GLFW_KEY_F17:
		return key::f17;
	case GLFW_KEY_F18:
		return key::f18;
	case GLFW_KEY_F19:
		return key::f19;
	case GLFW_KEY_F20:
		return key::f20;
	case GLFW_KEY_F21:
		return key::f21;
	case GLFW_KEY_F22:
		return key::f22;
	case GLFW_KEY_F23:
		return key::f23;
	case GLFW_KEY_F24:
		return key::f24;
	case GLFW_KEY_F25:
		return key::f25;
	case GLFW_KEY_KP_0:
		return key::kp_0;
	case GLFW_KEY_KP_1:
		return key::kp_1;
	case GLFW_KEY_KP_2:
		return key::kp_2;
	case GLFW_KEY_KP_3:
		return key::kp_3;
	case GLFW_KEY_KP_4:
		return key::kp_4;
	case GLFW_KEY_KP_5:
		return key::kp_5;
	case GLFW_KEY_KP_6:
		return key::kp_6;
	case GLFW_KEY_KP_7:
		return key::kp_7;
	case GLFW_KEY_KP_8:
		return key::kp_8;
	case GLFW_KEY_KP_9:
		return key::kp_9;
	case GLFW_KEY_KP_DECIMAL:
		return key::kp_decimal;
	case GLFW_KEY_KP_DIVIDE:
		return key::kp_divide;
	case GLFW_KEY_KP_MULTIPLY:
		return key::kp_multiply;
	case GLFW_KEY_KP_SUBTRACT:
		return key::kp_subtract;
	case GLFW_KEY_KP_ADD:
		return key::kp_add;
	case GLFW_KEY_KP_ENTER:
		return key::kp_enter;
	case GLFW_KEY_KP_EQUAL:
		return key::kp_equal;
	case GLFW_KEY_LEFT_SHIFT:
		return key::left_shift;
	case GLFW_KEY_LEFT_CONTROL:
		return key::left_control;
	case GLFW_KEY_LEFT_ALT:
		return key::left_alt;
	case GLFW_KEY_LEFT_SUPER:
		return key::left_super;
	case GLFW_KEY_RIGHT_SHIFT:
		return key::right_shift;
	case GLFW_KEY_RIGHT_CONTROL:
		return key::right_control;
	case GLFW_KEY_RIGHT_ALT:
		return key::right_alt;
	case GLFW_KEY_RIGHT_SUPER:
		return key::right_super;
	case GLFW_KEY_MENU:
		return key::menu;

	default:
		return key::unknown;
	}
}

key glfw_input::glfw_mouse_button_to_key(int const t_glfw_button)
{
	switch (t_glfw_button)
	{
		case GLFW_MOUSE_BUTTON_1:
			return key::mouse_button_1;
		case GLFW_MOUSE_BUTTON_2:
			return key::mouse_button_2;
		case GLFW_MOUSE_BUTTON_3:
			return key::mouse_button_middle;
		case GLFW_MOUSE_BUTTON_4:
			return key::mouse_button_4;
		case GLFW_MOUSE_BUTTON_5:
			return key::mouse_button_5;
		case GLFW_MOUSE_BUTTON_6:
			return key::mouse_button_6;
		case GLFW_MOUSE_BUTTON_7:
			return key::mouse_button_7;
		case GLFW_MOUSE_BUTTON_8:
			return key::mouse_button_8;
	}

	return key::mouse_button_1;
}

key_event glfw_input::glfw_key_event_to_key_event(int t_glfw_key_event)
{
	switch (t_glfw_key_event)
	{
	case GLFW_PRESS:
		return key_event::pressed;
	case GLFW_RELEASE:
		return key_event::released;
	case GLFW_REPEAT:
		return key_event::repeat;
	}

	return key_event::released;
}