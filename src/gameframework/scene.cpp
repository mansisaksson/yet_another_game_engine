#include "scene.h"
#include "entity.h"

void scene::add_entity(const transform& transform, const std::shared_ptr<entity>& t_entity)
{
	t_entity->current_scene = this;
	t_entity->transform = transform;
	entities.push_back(t_entity);
	t_entity->begin_play();
}

void scene::remove_entity(const entity* t_entity)
{
	entities.erase(
		std::remove_if(entities.begin(), entities.end(), [&](const std::shared_ptr<entity>& entity) { return entity.get() == t_entity; }),
		entities.end()
	);
}

void scene::tick_scene(float t_delta_time)
{
	// TODO: We don't need to tick all entities, only the ones who wants to get ticked
	for (const auto& entity : entities)
	{
		if (entity)
			entity->tick(t_delta_time);
	}
}

void scene::draw_scene(const viewport* viewport)
{
	for (const auto& entity : entities)
	{
		if (entity)
			entity->draw(viewport);
	}
}
