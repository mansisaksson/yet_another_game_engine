#include "gl_material.h"
#include "rendercore/texture.h"

gl_material::gl_material(const material_data& t_material_data)
	: material(t_material_data)
{
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(t_material_data.vertex_shader, GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(t_material_data.fragment_shader, GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(m_program, m_shaders[i]);

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}

gl_material::~gl_material()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);
}

void gl_material::bind()
{
	glUseProgram(m_program);

	for (size_t i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i])
			m_textures[i]->bind((uint32_t)i);
	}
}

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>
void gl_material::update(const transform& t_mesh_transform, const matrix4x4& t_view_matrix)
{
	struct local
	{
		static glm::mat4 GetModelMatrix()
		{
			const glm::mat4 posMatrix = glm::translate(glm::vec3(0, 0 ,0));
			const glm::mat4 rotXMatrix = glm::rotate(0.f, glm::vec3(1, 0, 0));
			const glm::mat4 rotYMatrix = glm::rotate(0.f, glm::vec3(0, 1, 0));
			const glm::mat4 rotZMatrix = glm::rotate(0.f, glm::vec3(0, 0, 1));
			const glm::mat4 scaleMatrix = glm::scale(glm::vec3(1, 1, 1));

			const glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

			return posMatrix * rotMatrix * scaleMatrix;
		}
	};

	const glm::vec3 pos(-10.f, 0, 0);
	const glm::vec3 forward = glm::vec3(1, 0, 0);
	const glm::vec3 up = glm::vec3(0, 0, 1);
	glm::mat4 m_perspective = glm::perspective(70.f, 1280.f / 720.f, 0.01f, 10000.f)
		* glm::lookAt(pos, pos + forward, up);

	const matrix4x4 model = t_view_matrix * t_mesh_transform.to_matrix();
	glUniformMatrix4fv(
		m_uniforms[TRANSFORM_U],
		1,
		/*If you write your own matrix class, you might need to transpose this to be in the order OpenGL expects*/GL_FALSE,
		&model[0][0]
	);
}

void gl_material::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
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

GLuint gl_material::CreateShader(const std::string& text, GLenum shaderType)
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