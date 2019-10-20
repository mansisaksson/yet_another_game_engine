#pragma once
#include "core/core.h"
#include "assetmanager/assetptr.h"

class viewport;
class material;

struct indexed_model
{
	std::vector<vector3> positions;
	std::vector<vector2> tex_coords;
	std::vector<vector3> normals;
	std::vector<uint32_t> indices;
	std::string material_ref;
};

class static_mesh : public std::enable_shared_from_this<static_mesh>
{
protected:
	std::vector<asset_ptr<material>> m_materials;

private:
	static_mesh() = delete;
	static_mesh(const static_mesh&) = delete;
	static_mesh(static_mesh&&) = delete;
	bool operator=(const static_mesh&) = delete;

protected:
	static_mesh(const indexed_model& model);

public:
	static std::shared_ptr<static_mesh> create_mesh(const indexed_model& t_model);

	virtual void draw(const transform &t_mesh_transform, viewport &t_viewport) = 0;

};