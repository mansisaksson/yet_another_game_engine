#include "tavlawindow.h"
#include "platform/platform.h"
#include "platform/platformwindow.h"
#include "rendercore/staticmesh.h"

void tavla_window::construct()
{
	m_window = platform::create_window();
	m_window->init(width, height, title);
	m_window->on_window_closed.bind(delegate<void, platform_window&>::create_weak_function(shared_from_this(), [&](platform_window&) {
		m_window.reset();
	}));

	static_mesh_ptr = asset_ptr<static_mesh>("basic_mesh");
}

void tavla_window::destruct()
{
	m_window->close_window();
	m_window.reset();
}

void tavla_window::tick(float t_delta_time)
{
	
}

void tavla_window::draw()
{
	if (!m_window)
		return;

	m_window->make_current();
	m_window->clear(color::blue);

	if (static_mesh_ptr)
		static_mesh_ptr->draw();

	m_window->swap_buffers();
}