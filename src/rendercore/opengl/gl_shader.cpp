#include "gl_shader.h"
#include "rendercore/texture.h"
#include <GLFW/glfw3.h>

gl_shader::gl_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(vertex_shader, GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(fragment_shader, GL_FRAGMENT_SHADER);

	for (uint32_t i = 0; i < 2; i++)
		glAttachShader(m_program, m_shaders[i]);

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "tex_coord");
	glBindAttribLocation(m_program, 2, "normal");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
}

gl_shader::~gl_shader()
{
	for (uint32_t i = 0; i < 2; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);
}

void gl_shader::bind()
{
	glUseProgram(m_program);
}

void gl_shader::set_uniform_float(const std::string& uniform_name, float value)
{
	const auto u_location = glGetUniformLocation(m_program, uniform_name.c_str());
	if (u_location != -1)
	{
		glUniform1f(u_location, value);
	}
}

void gl_shader::set_uniform_vector4(const std::string& uniform_name, const vector4& value)
{
	const auto u_location = glGetUniformLocation(m_program, uniform_name.c_str());
	if (u_location != -1)
	{
		glUniform4f(u_location, value[0], value[1], value[2], value[3]);
	}
}

void gl_shader::set_uniform_matrix4x4(const std::string& uniform_name, const matrix4x4& value)
{
	const auto u_location = glGetUniformLocation(m_program, uniform_name.c_str());
	if (u_location != -1)
	{
		glUniformMatrix4fv(
			u_location,
			1,
			GL_TRUE, // Transpose from row-major -> column-major
			&value[0][0]
		);
	}
}

void gl_shader::set_uniform_texture(const std::string& uniform_name, const texture* texture)
{
	const auto u_location = glGetUniformLocation(m_program, uniform_name.c_str());
	if (u_location != -1)
	{
		glUniform1i(u_location, texture->get_texture_unit());
	}
}

void gl_shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		log::error("gl_material", "%s: '%s'", errorMessage.c_str(), error);
	}
}

GLuint gl_shader::CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		log::error("gl_material", "Error: Shader creation failed!");

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = (GLint)text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}