#include "platforminput.h"
#include <algorithm>

guid platform_input::add_input_listener(std::weak_ptr<void> t_owner, const std::function<void(const input_event&)> & t_callback)
{
	if (t_owner.expired())
		return guid();

	const guid guid = guid::new_guid();
	input_listeners.push_back({ guid, t_owner, t_callback });
	return guid;
}

void platform_input::remove_input_listener(const guid & t_guid)
{
	input_listeners.erase(
		std::remove_if(input_listeners.begin(), input_listeners.end(), [&](const auto& listner)->bool { return listner.id == t_guid; }),
		input_listeners.end()
	);
}

void platform_input::generate_input_event(const input_event & t_input_event)
{
	input_listeners.erase(
		std::remove_if(input_listeners.begin(), input_listeners.end(), [&](const auto& listner)->bool { return listner.owner.expired(); }),
		input_listeners.end()
	);

	for (const auto & listener : input_listeners)
	{
		listener.callback(t_input_event);
	}
}

bool platform_input::is_key_down(key const t_key) const
{
	return std::find(pressed_keys.begin(), pressed_keys.end(), t_key) != std::end(pressed_keys);
}