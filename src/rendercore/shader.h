#pragma once
#include "core/core.h"

class texture;

class shader : public std::enable_shared_from_this<shader>
{
private:
	shader(const shader&) = delete;
	shader(shader&&) = delete;
	bool operator=(const shader&) = delete;

protected:
	shader() {};

public:
	static std::shared_ptr<shader> create_shader(const std::string& vertex_shader, const std::string& fragment_shader);

	virtual void bind() = 0;
	virtual void set_uniform_float(const std::string& uniform_name, float value) = 0;
	virtual void set_uniform_vector4(const std::string& uniform_name, const vector4& value) = 0;
	virtual void set_uniform_matrix4x4(const std::string& uniform_name, const matrix4x4& value) = 0;
	virtual void set_uniform_texture(const std::string& uniform_name, const texture* texture) = 0;

};