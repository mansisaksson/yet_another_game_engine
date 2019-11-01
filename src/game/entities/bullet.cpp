#include "bullet.h"
#include "rendercore/staticmesh.h"
#include "assetmanager/assetmanager.h"

bullet_entity::bullet_entity(const vector3& velocity, float lifetime)
	: m_velocity(velocity)
	, m_lifetime(lifetime)
{

}

void bullet_entity::begin_play()
{
	m_bullet_mesh = asset_manager::get().load_asset<static_mesh>("/meshes/basic_mesh");
}

void bullet_entity::tick(float delta_time)
{
	transform.location += m_velocity * delta_time;
	m_lifetime -= delta_time;
	if (m_lifetime < 0.f)
		remove_from_scene();
}

void bullet_entity::draw(const viewport* viewport)
{
	if (m_bullet_mesh)
		m_bullet_mesh->draw(transform, viewport);
}
