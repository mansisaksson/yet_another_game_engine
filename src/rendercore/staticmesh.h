#pragma once
#include "core/core.h"

class viewport;
class material;

struct indexed_model
{
	std::string mesh_name;
	std::vector<vector3> positions;
	std::vector<vector2> tex_coords;
	std::vector<vector3> normals;
	std::vector<uint32_t> indices;
	std::string material_ref;
};

class static_mesh : public std::enable_shared_from_this<static_mesh>
{
protected:
	std::vector<std::shared_ptr<material>> m_materials;

private:
	static_mesh() = delete;
	static_mesh(const static_mesh&) = delete;
	static_mesh(static_mesh&&) = delete;
	bool operator=(const static_mesh&) = delete;

protected:
	static_mesh(const std::vector<indexed_model>& t_models);

public:
	static std::shared_ptr<static_mesh> create_mesh(const std::vector<indexed_model>& t_models);

	virtual void draw(const transform &t_mesh_transform, const viewport* t_viewport) = 0;

};