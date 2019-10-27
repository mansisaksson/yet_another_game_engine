#include "entity.h"
#include "assetmanager/assetptr.h"
#include "rendercore/viewport.h"
#include "rendercore/staticmesh.h"

void entity::draw(const viewport* viewport)
{
	if (!static_mesh_ptr)
		static_mesh_ptr = asset_ptr<static_mesh>("arrows_mesh");

	if (static_mesh_ptr)
		static_mesh_ptr->draw(transform, viewport);
}