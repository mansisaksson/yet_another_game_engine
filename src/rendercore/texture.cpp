#include "texture.h"
#include "opengl/gl_texture.h"

texture::texture(const texture_data& t_texture_data)
{
	init_texture(t_texture_data);
}

texture::~texture()
{
	destroy_texture();
}

std::shared_ptr<texture> texture::create_texture(const texture_data& t_texture_data)
{
	return std::make_shared<gl_texture>(t_texture_data)->shared_from_this();
}
