#pragma once
#include "core/core.h"
#include "rendercore/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class gl_shader : public shader
{
private:

	GLuint m_program = 0;
	GLuint m_shaders[2];

private:
	gl_shader() = delete;
	gl_shader(const gl_shader&) = delete;
	gl_shader(gl_shader&&) = delete;
	bool operator=(const gl_shader&) = delete;

public:

	gl_shader(const std::string& vertex_shader, const std::string& fragment_shader);
	~gl_shader();

	virtual void bind() override;
	virtual void set_uniform_float(const std::string& uniform_name, float value) override;
	virtual void set_uniform_vector4(const std::string& uniform_name, const vector4& value) override;
	virtual void set_uniform_matrix4x4(const std::string &uniform_name, const matrix4x4& value) override;
	virtual void set_uniform_texture(const std::string& uniform_name, const texture* texture) override;

private:

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, GLenum shaderType);

};