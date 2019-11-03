#include "asteroid.h"
#include "rendercore/staticmesh.h"
#include "assetmanager/assetmanager.h"

asteroid_entity::asteroid_entity(const vector3& velocity, float lifetime)
	: m_velocity(velocity)
	, m_lifetime(lifetime)
{

}

void asteroid_entity::begin_play()
{
	m_asteroid_mesh = asset_manager::get().load_asset<static_mesh>("/meshes/asteroid_mesh");
}

void asteroid_entity::tick(float delta_time)
{
	transform.location += m_velocity * delta_time;
	m_lifetime -= delta_time;
	if (m_lifetime < 0.f)
		remove_from_scene();
}

void asteroid_entity::draw(const viewport* viewport)
{
	if (m_asteroid_mesh)
		m_asteroid_mesh->draw(transform, viewport);
}
