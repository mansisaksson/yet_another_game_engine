#include "material.h"
#include "rendercore/texture.h"
#include "rendercore/shader.h"

material::material(const std::shared_ptr<shader>& shader, const unform_values& t_uniforms)
	: m_shader(shader)
	, m_uniforms(t_uniforms)
{

}

std::shared_ptr<material> material::create_material(const std::shared_ptr<shader>& shader, const unform_values& t_uniforms)
{
	return std::make_shared<material>(shader, t_uniforms);
}

void material::apply_material()
{
	m_shader->bind();

	for (auto& float_uniform : m_uniforms.float_uniforms)
	{
		m_shader->set_uniform_float(float_uniform.first, float_uniform.second);
	}

	for (auto& vec4_uniform : m_uniforms.vector4_uniforms)
	{
		m_shader->set_uniform_vector4(vec4_uniform.first, vec4_uniform.second);
	}

	for (auto& mat4_uniform : m_uniforms.matrix4x4_uniforms)
	{
		m_shader->set_uniform_matrix4x4(mat4_uniform.first, mat4_uniform.second);
	}

	uint32_t unit = 0;
	for (auto& tex_uniform : m_uniforms.texture_uniforms)
	{
		if (tex_uniform.second)
		{
			tex_uniform.second->bind((uint32_t)unit);
			unit++;

			m_shader->set_uniform_texture(tex_uniform.first, tex_uniform.second.get());
		}
	}
}

void material::set_uniform_float(const std::string& uniform_name, float value)
{
	m_uniforms.float_uniforms[uniform_name] = value;
	//m_shader->set_uniform_float(uniform_name, value);
}

void material::set_uniform_vector4(const std::string& uniform_name, const vector4& value)
{
	m_uniforms.vector4_uniforms[uniform_name] = value;
	//m_shader->set_uniform_vector4(uniform_name, value);
}

void material::set_uniform_matrix4x4(const std::string& uniform_name, const matrix4x4& value)
{
	m_uniforms.matrix4x4_uniforms[uniform_name] = value;
	//m_shader->set_uniform_matrix4x4(uniform_name, value);
}

void material::set_uniform_texture(const std::string& uniform_name, const std::shared_ptr<texture>& value)
{
	m_uniforms.texture_uniforms[uniform_name] = value;
	//m_shader->set_uniform_texture(uniform_name, value.get());
}