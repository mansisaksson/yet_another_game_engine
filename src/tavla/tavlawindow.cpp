#include "tavlawindow.h"
#include "../platform/platform.h"
#include "../platform/platformwindow.h"

tavla_window::tavla_window()
	: height(640)
	, width(480)
{
	window = platform::create_window();
}

tavla_window::~tavla_window()
{
	window->close_window();
}

void tavla_window::tick(float t_delta_time)
{
	
}