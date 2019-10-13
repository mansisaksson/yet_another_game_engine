#include "staticmesh.h"

static_mesh::static_mesh(const indexed_model& model)
{

}

static_mesh::~static_mesh()
{
	destroy_mesh();
}

std::shared_ptr<static_mesh> static_mesh::create_mesh(const indexed_model& model)
{
	return nullptr;
}

std::shared_ptr<static_mesh> static_mesh::load_mesh(const std::string& file_path)
{
	return nullptr;
}