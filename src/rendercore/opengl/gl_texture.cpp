#include "gl_texture.h"
#include <cassert>

gl_texture::gl_texture(const texture_data& t_texture_data)
	: texture(t_texture_data)
{
}

void gl_texture::bind(uint32_t unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void gl_texture::init_texture(const texture_data& t_texture_data)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	const auto to_gl_color_format = [](auto color_format)
	{
		switch (color_format)
		{
		case texture_format::RGBA:
			return GL_RGBA;
		case texture_format::RGB:
			return GL_RGB;
		case texture_format::GREYSCALE:
			return GL_RED;
		}
		return GL_RED;
	};

	glTexImage2D(
		GL_TEXTURE_2D,
		/*MIP LEVEL*/0,
		to_gl_color_format(t_texture_data.render_format),
		t_texture_data.width,
		t_texture_data.height,
		0,
		to_gl_color_format(t_texture_data.data_format),
		GL_UNSIGNED_BYTE,
		&t_texture_data.color_data[0]
	);
}

void gl_texture::destroy_texture()
{
	glDeleteTextures(1, &m_texture);
}