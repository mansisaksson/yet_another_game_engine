#include "sky.h"
#include "rendercore/staticmesh.h"
#include "assetmanager/assetmanager.h"
#include "gameframework/scene.h"
#include "bullet.h"

void sky_entity::begin_play()
{
	m_static_mesh_ptr = asset_manager::get().load_asset<static_mesh>("/meshes/sky_mesh");
}

void sky_entity::draw(const viewport* viewport)
{
	if (m_static_mesh_ptr)
		m_static_mesh_ptr->draw(transform, viewport);
}
