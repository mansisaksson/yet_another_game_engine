#include "assetloaders.h"
#include "core/paths.h"
#include "rendercore/staticmesh.h"
#include "rendercore/texture.h"
#include "rendercore/material.h"

#include <iostream>
#include <fstream>
#include <streambuf>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "loaders/obj_loader.h"
#include "loaders/stb_image.h"

struct local
{
	static std::string parse_string(const json& json_object, const std::string& key)
	{
		const auto asset_type_field = json_object.find(key);
		if (asset_type_field != json_object.end())
		{
			const auto asset_type_value = asset_type_field.value();
			if (asset_type_value.is_string())
				return asset_type_value.get<std::string>();
		}
		return std::string();
	}

	static std::vector<std::string> parse_string_array(const json& json_object, const std::string& key)
	{
		const auto asset_type_field = json_object.find(key);
		if (asset_type_field != json_object.end())
		{
			const auto asset_type_value = asset_type_field.value();
			if (asset_type_value.is_array())
			{
				return asset_type_value.get<std::vector<std::string>>();
			}
		}
		return {};
	}

	static std::string load_file_as_string(const std::string& t_file_path)
	{
		std::ifstream t(t_file_path);
		if (!t.good())
		{
			log::error("asset_loader", "could not find file: %s", t_file_path.c_str());
			return "";
		}

		std::string str;

		t.seekg(0, std::ios::end);
		str.reserve((size_t)t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		return str;
	}
};

std::shared_ptr<static_mesh> asset_loader<static_mesh>::load_asset(const std::string& t_asset_path)
{
	const std::string asset_path = paths::combine_paths(paths::content_asset_dir(), t_asset_path + ".json");
	const std::string asset_data_string = local::load_file_as_string(asset_path);
	if (asset_data_string.empty())
	{
		return nullptr;
	}

	const auto json_asset = json::parse(asset_data_string);
	
	const std::string asset_type = local::parse_string(json_asset, "asset_type");
	if (asset_type != "static_mesh")
	{
		log::error("asset_loader", "wrong asset_type: %s", asset_type.c_str());
		return nullptr;
	}
	
	indexed_model indexed_model;

	/*
	TODO:
		Simple .OBJ loader that I found on the internet
		Is missing a whole lot of error checking and other features, such as multiple materials
	*/
	const std::string data_asset = local::parse_string(json_asset, "data_asset");
	const std::string data_asset_path = paths::combine_paths(paths::content_dir(), data_asset);
	const obj_model obj_model(data_asset_path);
	indexed_model = obj_model.to_indexed_model();

	const std::vector<std::string> materials = local::parse_string_array(json_asset, "materials");
	indexed_model.material_ref = materials.size() > 0 ? materials[0] : "basic_mat";

	return static_mesh::create_mesh(indexed_model)->shared_from_this();
}

std::shared_ptr<texture> asset_loader<texture>::load_asset(const std::string& t_asset_path)
{
	const std::string asset_path = paths::combine_paths(paths::content_asset_dir(), t_asset_path + ".json");
	const std::string asset_data_string = local::load_file_as_string(asset_path);
	if (asset_data_string.empty())
	{
		return nullptr;
	}

	const auto json_asset = json::parse(asset_data_string);

	const std::string asset_type = local::parse_string(json_asset, "asset_type");
	if (asset_type != "texture")
	{
		log::error("asset_loader", "wrong asset_type: %s", asset_type.c_str());
		return nullptr;
	}

	const std::string data_asset = local::parse_string(json_asset, "data_asset");
	//const std::string data_format = local::parse_string(json_asset, "data_format");
	//const std::string render_format = local::parse_string(json_asset, "render_format");

	const std::string data_asset_path = paths::combine_paths(paths::content_dir(), data_asset);
	
	texture_data tex_data;
	int num_comp;
	stbi_uc* const image_data = stbi_load(data_asset_path.c_str(), &tex_data.width, &tex_data.height, &num_comp, 4);
	
	if (image_data == nullptr)
	{
		log::error("asset_loader", "wrong asset_type: (%s) %s", asset_type.c_str(), t_asset_path.c_str());
		return nullptr;
	}

	// TODO: should allow for different texture formats
	//if (num_comp == 1)
	//	tex_data.data_format = texture_format::GREYSCALE;
	//else if (num_comp == 3)
	//	tex_data.data_format = texture_format::RGB;
	//else
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

std::shared_ptr<material> asset_loader<material>::load_asset(const std::string& t_asset_path)
{
	const std::string asset_path = paths::combine_paths(paths::content_asset_dir(), t_asset_path + ".json");
	const std::string asset_data_string = local::load_file_as_string(asset_path);
	if (asset_data_string.empty())
	{
		return nullptr;
	}

	const auto json_asset = json::parse(asset_data_string);

	const std::string asset_type = local::parse_string(json_asset, "asset_type");
	if (asset_type != "material")
	{
		log::error("asset_loader", "wrong asset_type: (%s) %s", asset_type.c_str(), t_asset_path.c_str());
		return nullptr;
	}

	material_data mat_data;

	const std::string fragment_shader_asset = local::parse_string(json_asset, "fragment_shader");
	mat_data.fragment_shader = local::load_file_as_string(paths::combine_paths(paths::content_dir(), fragment_shader_asset));

	const std::string vertex_shader_asset = local::parse_string(json_asset, "vertex_shader");
	mat_data.vertex_shader = local::load_file_as_string(paths::combine_paths(paths::content_dir(), vertex_shader_asset));

	mat_data.textures = local::parse_string_array(json_asset, "textures");

	return material::create_material(mat_data);
}