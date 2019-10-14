#include "staticmesh.h"
#include "opengl/gl_staticmesh.h"

static_mesh::static_mesh(const indexed_model& t_model)
{
	init_mesh(t_model);
}

static_mesh::~static_mesh()
{
	destroy_mesh();
}

std::shared_ptr<static_mesh> static_mesh::create_mesh(const indexed_model& t_model)
{
	return std::make_shared<gl_static_mesh>(t_model)->shared_from_this();
}
