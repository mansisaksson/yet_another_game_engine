#include "physicscube.h"
#include "rendercore/staticmesh.h"
#include "assetmanager/assetmanager.h"
#include "gameframework/scene.h"

#include "physicscore/physicscore.h"

void physics_cube_entity::begin_play()
{
	m_static_mesh_ptr = asset_manager::get().load_asset<static_mesh>("/meshes/cube_mesh");

	entity_transform.scale = m_cube_size;

	const box_shape box_collision(vector3(m_cube_size / 2.f, m_cube_size / 2.f, m_cube_size / 2.f));

	m_rigid_body = std::make_shared<rigid_body>(&entity_transform);
	m_rigid_body->set_mass(m_mass);
	m_rigid_body->add_box_shape(box_collision, transform::identity);
	m_rigid_body->set_collision_type(m_simulate_physics ? collision_type::simulated : collision_type::world_static);
	m_rigid_body->set_collision_channel(collision_channel::channel_1);
	m_rigid_body->set_collision_response_to_channel(collision_channel::channel_1, collision_response::block_and_overlap);

	get_scene()->get_physics_scene()->add_rigid_body(*m_rigid_body.get());

	//m_rigid_body->on_hit.bind(rigid_body::on_hit_delegate::create_function([&](const std::vector<hit_result> &hit_result) 
	//{
	//	log::info("tmp", "Hit!");
	//}));

	//m_rigid_body->on_begin_overlap.bind(rigid_body::on_overlap_delegate::create_function([&](rigid_body &other_body) 
	//{
	//	log::info("tmp", "Begin Overlap");
	//}));
	//
	//m_rigid_body->on_end_overlap.bind(rigid_body::on_overlap_delegate::create_function([&](rigid_body &other_body) 
	//{
	//	log::info("tmp", "End Overlap");
	//}));
}

void physics_cube_entity::removed_from_scene(scene* scene)
{
	scene->get_physics_scene()->remove_rigid_body(*m_rigid_body.get());
}

void physics_cube_entity::draw(const viewport* viewport)
{
	if (m_static_mesh_ptr)
		m_static_mesh_ptr->draw(entity_transform, viewport);
}
