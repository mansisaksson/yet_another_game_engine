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

	virtual void bind(uint32_t unit) override;

protected:

	virtual void init_texture(const texture_data& t_texture_data) override;
	virtual void destroy_texture() override;
};