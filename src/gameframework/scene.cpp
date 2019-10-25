#include "scene.h"
#include "entity.h"

void scene::draw_scene(const viewport* viewport)
{
	for (const auto& entity : entities)
	{
		if (entity)
			entity->draw(viewport);
	}
}
