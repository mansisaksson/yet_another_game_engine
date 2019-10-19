#include "staticmesh.h"
#include "opengl/gl_staticmesh.h"

static_mesh::static_mesh(const indexed_model& t_model)
{
	m_materials.push_back(asset_ptr<material>(t_model.material_ref));
}

std::shared_ptr<static_mesh> static_mesh::create_mesh(const indexed_model& t_model)
{
	return std::make_shared<gl_static_mesh>(t_model)->shared_from_this();
}
