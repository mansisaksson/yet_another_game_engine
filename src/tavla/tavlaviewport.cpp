#include "tavlaviewport.h"
#include "tavlawindow.h"
#include "rendercore/staticmesh.h"
#include "rendercore/viewport.h"

void tavla_viewport::construct()
{
	m_viewport = std::make_shared<viewport>(vector3::zero, 70.f, 1280.f / 720.f, 0.01f, 10000.f);
	static_mesh_ptr = asset_ptr<static_mesh>("basic_mesh");
}

void tavla_viewport::destruct()
{

}

void tavla_viewport::tick(float t_delta_time)
{

}

#include <GL/glew.h>
void tavla_viewport::draw()
{
	const auto parent_window = tavla_window::find_parent_window(weak_from_this());
	if (parent_window.expired() || !parent_window.lock()->has_valid_context())
	{
		log::error("tavla_viewport", "Viewport not attached to a window, cannot draw!");
		return;
	}

	const auto window_size = parent_window.lock()->get_window_size();
	
	m_viewport->update_viewport(vector3::zero, 70.f, (float)std::get<0>(window_size) / (float)std::get<1>(window_size), 0.01f, 10000.f);

	// TODO: Move this to something like gl_viewport
	glViewport(0, 0, std::get<0>(window_size), std::get<1>(window_size));

	if (static_mesh_ptr)
		static_mesh_ptr->draw(transform::identity, *m_viewport);
}