#pragma once
#include <memory>
#include "vertex.h"

enum class texture_format
{
	RGBA,
	RGB,
	GREYSCALE
};

struct texture_data
{
	texture_format data_format;
	texture_format render_format;
	int width;
	int height;
	std::vector<uint8_t> color_data;
};

class texture : public std::enable_shared_from_this<texture>
{
private:
	texture() = delete;
	texture(const texture&) = delete;
	texture(texture&&) = delete;
	bool operator=(const texture&) = delete;

protected:

	texture(const texture_data& model);
	virtual ~texture();

public:
	static std::shared_ptr<texture> create_texture(const texture_data& t_texture_data);

	virtual void bind(uint32_t unit) = 0;

protected:

	virtual void init_texture(const texture_data& t_texture_data) {};
	virtual void destroy_texture() {};

};