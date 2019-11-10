#include "player.h"
#include "rendercore/staticmesh.h"
#include "assetmanager/assetmanager.h"
#include "gameframework/scene.h"
#include "bullet.h"

void player_entity::begin_play()
{
	m_static_mesh_ptr = asset_manager::get().load_asset<static_mesh>("/meshes/ship_mesh");
}

void player_entity::tick(float delta_time)
{
	entity_transform.location += vector3(m_movement_input.y * delta_time, m_movement_input.x * delta_time, 0) * m_movement_speed;
}

void player_entity::draw(const viewport* viewport)
{
	if (m_static_mesh_ptr)
		m_static_mesh_ptr->draw(entity_transform, viewport);
}

void player_entity::fire()
{
	const auto ship_forward = entity_transform.rotation.get_forward();
	get_scene()->spawn_entity<bullet_entity>(entity_transform, ship_forward * 3.f, 5.f);
}

void player_entity::set_movement_vector(const vector2& new_mvn_vector)
{
	m_movement_input = new_mvn_vector;
}