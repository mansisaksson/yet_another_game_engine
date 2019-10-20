#pragma once
#include "core/core.h"
#include "assetmanager/assetptr.h"

class texture;

struct material_data
{
	std::string vertex_shader;
	std::string fragment_shader;
	std::vector<std::string> textures;
};

class material : public std::enable_shared_from_this<material>
{
protected:
	std::vector<asset_ptr<texture>> m_textures;

private:
	material() = delete;
	material(const material&) = delete;
	material(material&&) = delete;
	bool operator=(const material&) = delete;

protected:

	material(const material_data &t_material_data);

public:
	static std::shared_ptr<material> create_material(const material_data& t_material_data);

	virtual void bind() = 0;
	virtual void update(const transform &t_mesh_transform, const matrix4x4& t_view_matrix) = 0;

};