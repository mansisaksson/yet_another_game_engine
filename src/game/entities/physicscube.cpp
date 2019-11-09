#include "physicscube.h"
#include "rendercore/staticmesh.h"
#include "assetmanager/assetmanager.h"
#include "gameframework/scene.h"

#include "physicscore/physicscore.h"

void physics_cube_entity::begin_play()
{
	m_static_mesh_ptr = asset_manager::get().load_asset<static_mesh>("/meshes/cube_mesh");

	transform.scale = m_cube_size;

	const box_shape box_collision(vector3(m_cube_size / 2.f, m_cube_size / 2.f, m_cube_size / 2.f));

	m_rigid_body = std::make_shared<rigid_body>();
	m_rigid_body->set_mass(m_mass);
	m_rigid_body->add_box_shape(box_collision, transform::identity);
	m_rigid_body->set_simulate_physics(m_simulate_physics);
	
	// TODO: should have a btSyncScene thing here
	m_rigid_body->set_world_location(transform.location);
	m_rigid_body->set_world_rotation(transform.rotation);

	get_scene()->get_physics_scene()->add_rigid_body(*m_rigid_body.get());
}

void physics_cube_entity::removed_from_scene(scene* scene)
{
	scene->get_physics_scene()->add_rigid_body(*m_rigid_body.get());
}

void physics_cube_entity::tick(float delta_time)
{
	
}

void physics_cube_entity::post_physics_tick()
{
	//log::info("physics_cube_entity", "Location: %s", m_rigid_body->get_rigid_body_location().to_string().c_str());

	transform.location = m_rigid_body->get_rigid_body_location();
	transform.rotation = m_rigid_body->get_rigid_body_rotation();
}

void physics_cube_entity::draw(const viewport* viewport)
{
	if (m_static_mesh_ptr)
		m_static_mesh_ptr->draw(transform, viewport);
}
