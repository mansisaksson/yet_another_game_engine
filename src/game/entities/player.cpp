#include "player.h"
#include "rendercore/staticmesh.h"
#include "assetmanager/assetmanager.h"

void player_entity::begin_play()
{
	m_static_mesh_ptr = asset_manager::get().load_asset<static_mesh>("debug_mesh");
}

void player_entity::tick(float delta_time)
{
	m_transform.location += vector3(m_movement_input.x * delta_time, m_movement_input.y * delta_time, 0);
}

void player_entity::draw(const viewport* viewport)
{
	if (m_static_mesh_ptr)
		m_static_mesh_ptr->draw(m_transform, viewport);
}

void player_entity::set_movement_vector(const vector2& new_mvn_vector)
{
	m_movement_input = new_mvn_vector;
}