#include "platforminput.h"
#include <algorithm>

guid platform_input::add_input_event(std::weak_ptr<void> t_owner, const std::function<void(const input_event&)> & t_callback)
{
	return guid::new_guid();
}

void platform_input::remove_input_event(const guid & t_guid)
{

}

void platform_input::generate_input_event(const input_event & t_input_event)
{

}

bool platform_input::is_key_down(key const t_key) const
{
	return std::find(pressed_keys.begin(), pressed_keys.end(), t_key) != std::end(pressed_keys);
}