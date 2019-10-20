#include "tavlaviewport.h"
#include "tavlawindow.h"
#include "rendercore/staticmesh.h"

void tavla_viewport::construct()
{
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
	if (parent_window.expired())
	{
		log::error("tavla_viewport", "Viewport not attached to a window, cannot draw!");
		return;
	}

	const auto window_size = parent_window.lock()->get_window_size();
	
	// TODO: Move this to something like gl_viewport
	glViewport(0, 0, std::get<0>(window_size), std::get<1>(window_size));

	if (static_mesh_ptr)
		static_mesh_ptr->draw();
}