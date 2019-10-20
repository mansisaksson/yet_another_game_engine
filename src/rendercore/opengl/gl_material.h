#pragma once
#include "core/core.h"
#include "rendercore/material.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class gl_material : public material
{
private:
	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	static const unsigned int NUM_SHADERS = 2;

	GLuint m_program = 0;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];

private:
	gl_material() = delete;
	gl_material(const gl_material&) = delete;
	gl_material(gl_material&&) = delete;
	bool operator=(const gl_material&) = delete;

public:

	gl_material(const material_data &t_material_data);
	~gl_material();

	virtual void bind() override;
	virtual void update(const transform &t_mesh_transform, const matrix4x4& t_view_matrix) override;

private:

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, GLenum shaderType);

};