#include "staticmesh.h"
#include "opengl/gl_staticmesh.h"

static_mesh::static_mesh(const indexed_model& model)
{

}

static_mesh::~static_mesh()
{
	destroy_mesh();
}

std::shared_ptr<static_mesh> static_mesh::create_mesh(const indexed_model& model)
{
	return std::make_shared<gl_static_mesh>(model)->shared_from_this();
}
