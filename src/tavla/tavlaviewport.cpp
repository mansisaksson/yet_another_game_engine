#include "tavlaviewport.h"
#include "tavlawindow.h"
#include "rendercore/staticmesh.h"
#include "rendercore/viewport.h"
#include "gameframework/scene.h"

tavla_viewport::tavla_viewport()
	: fov(70.f)
	, near_plane(0.01f)
	, far_plane(10000.f)
{

}

void tavla_viewport::construct()
{
	m_viewport = viewport::create_viewport();
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

	const auto window_dimensions = parent_window.lock()->get_window_size();
	const dimensions viewport_dimensions = get_tavla_dimentions();
	
	m_viewport->set_view_perspective(
		fov,
		(float)viewport_dimensions.width / (float)viewport_dimensions.height,
		near_plane,
		far_plane
	);

	m_viewport->make_current(
		viewport_dimensions.x_pos, 
		viewport_dimensions.y_pos, 
		viewport_dimensions.width, 
		viewport_dimensions.height,
		std::get<0>(window_dimensions),
		std::get<1>(window_dimensions)
	);

	if (!scene.expired())
		scene.lock()->draw_scene(m_viewport.get());
}

viewport* tavla_viewport::get_viewport() const
{
	return m_viewport.get();
}