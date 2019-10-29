#pragma once
#include "core/core.h"
#include <functional>
#include <memory>

class platform_window;

struct input_event
{
	key key;
	key_event key_event;
};

class platform_input
{
private:
	std::vector<key> pressed_keys;

	multicast_delegate<const input_event&> on_input_event;

public:

	guid add_input_listener(const std::weak_ptr<void> &t_owner, const std::function<void(const input_event&)>& t_callback);

	void remove_input_listener(const guid &t_guid);

	void generate_input_event(const input_event &t_input_event);

	bool is_key_down(key const t_key) const;

	// ~begin platform_input interface
	virtual void update_input() = 0;
	virtual std::tuple<int, int> get_cursor_location(const platform_window& t_window) = 0;
	// ~end platform_input interface
};

