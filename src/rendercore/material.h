#pragma once
#include "core/core.h"

class texture;
class shader;

struct unform_values
{
	std::unordered_map<std::string, float> float_uniforms;
	std::unordered_map<std::string, vector4> vector4_uniforms;
	std::unordered_map<std::string, matrix4x4> matrix4x4_uniforms;
	std::unordered_map<std::string, std::shared_ptr<texture>> texture_uniforms;
};

class material : public std::enable_shared_from_this<material>
{
protected:
	unform_values m_uniforms;
	std::shared_ptr<shader> m_shader;

private:
	material() = delete;
	material(const material&) = delete;
	material(material&&) = delete;
	bool operator=(const material&) = delete;

public:

	material(const std::shared_ptr<shader>& shader, const unform_values& t_uniforms);

	static std::shared_ptr<material> create_material(const std::shared_ptr<shader>& shader, const unform_values& t_uniforms);

	virtual void set_uniform_float(const std::string& uniform_name, float value);
	virtual void set_uniform_vector4(const std::string& uniform_name, const vector4& value);
	virtual void set_uniform_matrix4x4(const std::string& uniform_name, const matrix4x4& value);
	virtual void set_uniform_texture(const std::string& uniform_name, const std::shared_ptr<texture> &value);

	virtual void apply_material();

};