#include "entity.h"
#include "scene.h"

void entity::remove_from_scene()
{
	removed_from_scene(get_scene());
	get_scene()->remove_entity(this);
}