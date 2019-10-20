#include "tavlaviewport.h"
#include "tavlawindow.h"
#include "rendercore/staticmesh.h"
#include "rendercore/opengl/gl_viewport.h"

void tavla_viewport::construct()
{
	m_viewport = std::make_shared<gl_viewport>(vector3::zero, 70.f, 1280.f / 720.f, 0.01f, 10000.f);
	static_mesh_ptr = asset_ptr<static_mesh>("basic_mesh");
}

void tavla_viewport::destruct()
{

}

void tavla_viewport::tick(float t_delta_time)
{

}

void tavla_viewport::draw()
{
	const auto parent_window = tavla_window::find_parent_window(weak_from_this());
	if (parent_window.expired() || !parent_window.lock()->has_valid_context())
	{
		log::error("tavla_viewport", "Viewport not attached to a window, cannot draw!");
		return;
	}

	const dimensions viewport_dimensions = get_tavla_dimentions();
	
	m_viewport->update_viewport(vector3(-10, 0, 0), 70.f, (float)viewport_dimensions.width / (float)viewport_dimensions.height, 0.01f, 10000.f);
	m_viewport->make_current(viewport_dimensions.x_pos, viewport_dimensions.y_pos, viewport_dimensions.width, viewport_dimensions.height);

	if (static_mesh_ptr)
		static_mesh_ptr->draw(transform::identity, *m_viewport);
}