#include "material.h"
#include "rendercore/texture.h"

#include "opengl/gl_material.h"

material::material(const material_data& t_material_data)
{
	for (const auto tex_ref : t_material_data.textures)
	{
		m_textures.push_back(asset_ptr<texture>(tex_ref));
	}
}

std::shared_ptr<material> material::create_material(const material_data& t_material_data)
{
	return std::make_shared<gl_material>(t_material_data)->shared_from_this();
}