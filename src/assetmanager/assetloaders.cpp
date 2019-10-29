#include "assetloaders.h"
#include "core/paths.h"
#include "core/string.h"
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

	struct axis_conversion
	{
		int from_axis = -1;
		int to_axis = -1;
		float multiplier = 1.f;
	};

	const std::array<axis_conversion, 3> axis_conversions = [&]()
	{
		std::array<axis_conversion, 3> out_axis_conversions = { axis_conversion(), axis_conversion(), axis_conversion() };
		const auto axes_field = json_asset.find("axes");
		if (axes_field != json_asset.end() && axes_field.value().is_object())
		{
			const auto str_to_axis_conversion = [](int axis, const std::string & str)->axis_conversion
			{
				if (str.empty())
					return { -1, -1, 0 };

				if (yete_str::iequals(str, "+X") || yete_str::iequals(str, "X"))
					return { 0, axis, 1 };
				if (yete_str::iequals(str, "+Y") || yete_str::iequals(str, "Y"))
					return { 1, axis, 1 };
				if (yete_str::iequals(str, "+Z") || yete_str::iequals(str, "Z"))
					return { 2, axis, 1 };

				if (yete_str::iequals(str, "-X"))
					return { 0, axis, -1 };
				if (yete_str::iequals(str, "-Y"))
					return { 1, axis, -1 };
				if (yete_str::iequals(str, "-Z"))
					return { 2, axis, -1 };

				return { -1, -1, 0 };
			};
			out_axis_conversions[0] = str_to_axis_conversion(0, local::parse_string(axes_field.value(), "forward"));
			out_axis_conversions[1] = str_to_axis_conversion(1, local::parse_string(axes_field.value(), "right"));
			out_axis_conversions[2] = str_to_axis_conversion(2, local::parse_string(axes_field.value(), "up"));
		}
		return out_axis_conversions;
	}();

	/*
	TODO:
		Simple .OBJ loader that I found on the internet
		Is missing a whole lot of error checking and other features, such as multiple materials
	*/
	const std::string data_asset = local::parse_string(json_asset, "data_asset");
	const std::string data_asset_path = paths::combine_paths(paths::content_dir(), data_asset);
	const obj_model obj_model(data_asset_path);

	indexed_model model = obj_model.to_indexed_model();

	const auto convert_axes = [](indexed_model &model, const std::array<axis_conversion, 3> &axis_conversions)
	{
		vector3 tmp;

		// Convert positions
		for (vector3 &position : model.positions)
		{
			tmp = position;
			for (const auto &conversion : axis_conversions)
			{
				if (conversion.to_axis != -1 && conversion.from_axis != -1)
					position[conversion.to_axis] = tmp[conversion.from_axis] * conversion.multiplier;
			}
		}

		// Convert normals
		for (vector3& normal : model.normals)
		{
			tmp = normal;
			for (const auto& conversion : axis_conversions)
			{
				if (conversion.to_axis != -1 && conversion.from_axis != -1)
					normal[conversion.to_axis] = tmp[conversion.from_axis] * conversion.multiplier;
			}
		}
	};

	convert_axes(model, axis_conversions);

	const std::vector<std::string> materials = local::parse_string_array(json_asset, "materials");
	model.material_ref = materials.size() > 0 ? materials[0] : "basic_mat";

	return static_mesh::create_mesh(model);
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
	stbi_uc* const image_data = stbi_load(data_asset_path.c_str(), &tex_data.width, &tex_data.height, &num_comp, 0);
	
	if (image_data == nullptr)
	{
		log::error("asset_loader", "wrong asset_type: (%s) %s", asset_type.c_str(), t_asset_path.c_str());
		return nullptr;
	}

	if (num_comp == 0)
	{
		log::error("asset_loader", "failed to load image, no valid color data: (%s) %s", asset_type.c_str(), t_asset_path.c_str());
		return nullptr;
	}
	if (num_comp == 1)
	{
		tex_data.data_format = texture_format::GREYSCALE;
	}
	else if (num_comp == 3)
	{
		tex_data.data_format = texture_format::RGB;
	}
	else
	{
		tex_data.data_format = texture_format::RGBA;
	}
	
	tex_data.render_format = tex_data.data_format;
	
	const uint32_t num_bytes = tex_data.width * tex_data.height * num_comp;
	tex_data.color_data.reserve(num_bytes);
	for (size_t i = 0; i < num_bytes; i++)
	{
		tex_data.color_data.push_back(image_data[i]);
	}

	stbi_image_free(image_data);

	return texture::create_texture(tex_data);
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