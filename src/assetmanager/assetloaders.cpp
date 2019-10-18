#include "assetloaders.h"
#include "core/paths.h"

#include <iostream>
#include <filesystem>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "loaders/obj_loader.h"
#include "loaders/stb_image.h"

std::shared_ptr<static_mesh> asset_loader<static_mesh>::load_asset(const std::string& t_asset_path)
{
	/* 
	TODO:
		Simple .OBJ loader that I found on the internet
		Is missing a whole lot of error checking and other features
	*/

	const std::string full_path = paths::content_dir() + t_asset_path; // TODO: Combine paths
	const obj_model obj_model(full_path);
	const indexed_model indexed_model = obj_model.to_indexed_model();
	return static_mesh::create_mesh(indexed_model)->shared_from_this();
}

std::shared_ptr<texture> asset_loader<texture>::load_asset(const std::string& t_asset_path)
{
	const std::string full_path = paths::content_dir() + t_asset_path; // TODO: Combine paths

	texture_data tex_data;
	int num_comp;
	stbi_uc* image_data = stbi_load(full_path.c_str(), &tex_data.width, &tex_data.height, &num_comp, 4);
	
	if (image_data == nullptr)
	{
		std::cerr << "Texture loading failed for texture: " << t_asset_path << "\n";
		return nullptr;
	}

	if (num_comp == 1)
		tex_data.data_format = texture_format::GREYSCALE;
	else if (num_comp == 3)
		tex_data.data_format = texture_format::RGB;
	else
		tex_data.data_format = texture_format::RGBA;
	
	tex_data.render_format = tex_data.data_format;
	
	const uint32_t num_bytes = tex_data.width * tex_data.height * num_comp;
	tex_data.color_data.reserve(num_bytes);
	for (size_t i = 0; i < num_bytes; i++)
	{
		tex_data.color_data.push_back(image_data[i]);
	}

	stbi_image_free(image_data);

	return texture::create_texture(tex_data)->shared_from_this();
}