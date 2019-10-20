#include "tavlaapplication.h"
#include "platform/platform.h"
#include "platform/platforminput.h"

void tavla_application::construct()
{
	input_event_listen_id = platform::get_platform_input()->add_input_listener(weak_from_this(), [](const input_event &t_input_event)
	{
		log::info("tavla_application", "%s : %s", key_to_string(t_input_event.key).c_str(), key_event_to_string(t_input_event.key_event).c_str());
	});
}

void tavla_application::destruct()
{
	platform::get_platform_input()->remove_input_listener(input_event_listen_id);
}

void tavla_application::tick(float t_delta_time)
{
	platform::get_platform_input()->update_input();
}

int tavla_application::run()
{
	float old_time = platform::get_time();
	while (true)
	{
		const float new_time = platform::get_time();
		const float delta_time = new_time - old_time;

		tavla::calculate_tavla_tree_dimensions(shared_from_this());
		tavla::tick_tavla_tree(shared_from_this(), delta_time);
		tavla::draw_tavla_tree(shared_from_this());

		old_time = new_time;
	}

	return 0;
}