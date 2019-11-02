#include "staticmesh.h"
#include "opengl/gl_staticmesh.h"
#include "assetmanager/assetmanager.h"

static_mesh::static_mesh(const std::vector<indexed_model>& t_models)
{
	for (const auto &model : t_models)
	{
		auto mat = asset_manager::get().load_asset<material>(model.material_ref);
		if (!mat)
			mat = asset_manager::get().load_asset<material>("/materials/default_mat");

		m_materials.push_back(mat);
	}
}

std::shared_ptr<static_mesh> static_mesh::create_mesh(const std::vector<indexed_model>& t_models)
{
	return std::make_shared<gl_static_mesh>(t_models);
}
