#include "scene.h"
#include "entity.h"
#include "physicscore/physicscore.h"

scene::scene()
{
	m_physics_scene = std::make_shared<physics_scene>(vector3(0, 0, -10));
}

void scene::add_entity(const transform& transform, const std::shared_ptr<entity>& t_entity)
{
	t_entity->current_scene = this;
	t_entity->transform = transform;
	m_entities.push_back(t_entity);
	t_entity->begin_play();
}

void scene::remove_entity(const entity* t_entity)
{
	m_entities.erase(
		std::remove_if(m_entities.begin(), m_entities.end(), [&](const std::shared_ptr<entity>& entity) { return entity.get() == t_entity; }),
		m_entities.end()
	);
}

void scene::tick_scene(float t_delta_time)
{
	// TODO: We don't need to tick all entities, only the ones who wants to get ticked
	for (const auto& entity : m_entities)
	{
		if (entity)
			entity->tick(t_delta_time);
	}

	m_physics_scene->simulate(t_delta_time);

	for (const auto& entity : m_entities)
	{
		if (entity)
			entity->post_physics_tick();
	}
}

void scene::draw_scene(const viewport* viewport)
{
	for (const auto& entity : m_entities)
	{
		if (entity)
			entity->draw(viewport);
	}
}

physics_scene* scene::get_physics_scene() const
{
	return m_physics_scene.get();
}