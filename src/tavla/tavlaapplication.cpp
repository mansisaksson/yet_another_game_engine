#include "tavlaapplication.h"
#include "../platform/platform.h"
#include "../platform/platforminput.h"

tavla_application::tavla_application()
{

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
}