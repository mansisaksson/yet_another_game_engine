#include "tavlaapplication.h"
#include "platform/platform.h"
#include "platform/platforminput.h"

void tavla_application::construct()
{
	input_event_listen_id = platform::get_platform_input()->add_input_listener(shared_from_this(), [](const input_event &t_input_event)
		{
			std::cout << key_to_string(t_input_event.key) << " : " << key_event_to_string(t_input_event.key_event) << "\n";
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
	float time = 0;
	while (true)
	{
		tavla::tick_tavla_tree(shared_from_this(), time);
		time += 1;
	}

	return 0;
}