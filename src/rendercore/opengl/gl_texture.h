#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "rendercore/texture.h"

class gl_texture : public texture
{
private:
	GLuint m_texture;

private:
	gl_texture() = delete;
	gl_texture(const gl_texture&) = delete;
	gl_texture(gl_texture&&) = delete;
	bool operator=(const gl_texture&) = delete;

public:
	gl_texture(const texture_data& t_texture_data);
	~gl_texture();

	virtual void bind(uint32_t unit) override;

};