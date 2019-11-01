#include "scene.h"
#include "entity.h"

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
