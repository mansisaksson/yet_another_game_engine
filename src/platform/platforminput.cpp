#include "platforminput.h"
#include <algorithm>

guid platform_input::add_input_listener(const std::weak_ptr<void> & t_owner, const std::function<void(const input_event&)> & t_callback)
{
	if (t_owner.expired())
		return guid();

	return on_input_event.bind(delegate<void, const input_event&>::create_weak_function(t_owner, t_callback));
}

void platform_input::remove_input_listener(const guid & t_guid)
{
	on_input_event.remove(t_guid);
}

void platform_input::generate_input_event(const input_event & t_input_event)
{
	on_input_event.broadcast(t_input_event);
}

bool platform_input::is_key_down(key const t_key) const
{
	return std::find(pressed_keys.begin(), pressed_keys.end(), t_key) != std::end(pressed_keys);
}