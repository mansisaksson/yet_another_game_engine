#include "tavlawindow.h"
#include "../platform/platform.h"
#include "../platform/platformwindow.h"

void tavla_window::construct()
{
	m_window = platform::create_window();
	m_window->init(width, height, title);
}

void tavla_window::destruct()
{
	m_window->close_window();
}

void tavla_window::tick(float t_delta_time)
{
	
}