#include "shader.h"
#include "rendercore/texture.h"

#include "opengl/gl_shader.h"

std::shared_ptr<shader> shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	return std::make_shared<gl_shader>(vertex_shader, fragment_shader);
}