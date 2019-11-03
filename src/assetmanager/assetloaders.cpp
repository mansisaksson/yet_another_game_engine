#include "assetloaders.h"
#include "core/core.h"
#include "rendercore/staticmesh.h"
#include "rendercore/texture.h"
#include "rendercore/material.h"
#include "rendercore/shader.h"

#include <iostream>
#include <fstream>
#include <streambuf>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "loaders/obj_loader.h"
#include "loaders/stb_image.h"

struct local
{
	static std::pair<bool, std::string> try_parse_string(const json& json_object, const std::string& key)
	{
		const auto asset_type_field = json_object.find(key);
		if (asset_type_field != json_object.end())
		{
			const auto asset_type_value = asset_type_field.value();
			if (asset_type_value.is_string())
				return { true, asset_type_value.get<std::string>() };
		}
		return { false, {} };
	}

	static std::pair<bool, std::vector<std::string>> try_parse_string_array(const json& json_object, const std::string& key)
	{
		const auto asset_type_field = json_object.find(key);
		if (asset_type_field != json_object.end())
		{
			const auto asset_type_value = asset_type_field.value();
			if (asset_type_value.is_array())
			{
				// TODO: Validate that all elements are strngs
				return { true, asset_type_value.get<std::vector<std::string>>() };
			}
		}
		return { false, {} };
	}

	static std::pair<bool, std::unordered_map<std::string, std::string>> try_parse_string_map(const json& json_object, const std::string& key)
	{
		const auto asset_type_field = json_object.find(key);
		if (asset_type_field != json_object.end())
		{
			const auto asset_type_value = asset_type_field.value();
			if (asset_type_value.is_object())
			{
				// TODO: Validate that all elements are strngs
				return { true, asset_type_value.get<std::unordered_map<std::string, std::string>>() };
			}
		}
		return { false, {} };
	}

	static std::pair<bool, float> try_parse_float(const json& json_object, const std::string& key)
	{
		const auto asset_type_field = json_object.find(key);
		if (asset_type_field != json_object.end())
		{
			const auto asset_type_value = asset_type_field.value();
			if (asset_type_value.is_number())
			{
				return { true, asset_type_value.get<float>() };
			}
		}

		return { 0.f, false };
	}

	static std::pair<bool, std::array<float, 4>> try_parse_4d_float_array(const json& json_object, const std::string& key)
	{
		const auto asset_type_field = json_object.find(key);
		if (asset_type_field != json_object.end())
		{
			const auto asset_type_value = asset_type_field.value();
			if (asset_type_value.is_array())
			{
				if (asset_type_value.size() == 4)
					return { true, asset_type_value.get<std::array<float, 4>>() };
			}
		}

		return { false, {} };
	}

	static std::pair<bool, std::array<float, 16>> try_parse_16d_float_array(const json& json_object, const std::string& key)
	{
		const auto asset_type_field = json_object.find(key);
		if (asset_type_field != json_object.end())
		{
			const auto asset_type_value = asset_type_field.value();
			if (asset_type_value.is_array())
			{
				if (asset_type_value.size() == 16)
					return { true, asset_type_value.get<std::array<float, 16>>() };
			}
		}

		return { false, {} };
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
	
	const auto asset_type = local::try_parse_string(json_asset, "asset_type");
	if (!asset_type.first || asset_type.second != "static_mesh")
	{
		log::error("asset_loader", "wrong asset_type: %s", asset_type.second.c_str());
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
			out_axis_conversions[0] = str_to_axis_conversion(0, local::try_parse_string(axes_field.value(), "forward").second);
			out_axis_conversions[1] = str_to_axis_conversion(1, local::try_parse_string(axes_field.value(), "right").second);
			out_axis_conversions[2] = str_to_axis_conversion(2, local::try_parse_string(axes_field.value(), "up").second);
		}
		return out_axis_conversions;
	}();

	const auto data_asset = local::try_parse_string(json_asset, "data_asset");
	const std::string data_asset_path = paths::combine_paths(paths::data_asset_dir(), data_asset.second);
	
	std::vector<indexed_model> models = obj::obj_loader::load_obj(data_asset_path);

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

	const auto materials = local::try_parse_string_map(json_asset, "materials");
	for (indexed_model & model : models)
	{
		convert_axes(model, axis_conversions);

		const auto material_ref = materials.second.find(model.material_ref);
		if (material_ref != materials.second.end())
			model.material_ref = material_ref->second;
	}

	return static_mesh::create_mesh(models);
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

	const auto asset_type = local::try_parse_string(json_asset, "asset_type");
	if (!asset_type.first || asset_type.second != "texture")
	{
		log::error("asset_loader", "wrong asset_type: %s", asset_type.second.c_str());
		return nullptr;
	}

	const auto data_asset = local::try_parse_string(json_asset, "data_asset");
	//const std::string data_format = local::parse_string(json_asset, "data_format");
	//const std::string render_format = local::parse_string(json_asset, "render_format");

	const std::string data_asset_path = paths::combine_paths(paths::data_asset_dir(), data_asset.second);
	
	texture_data tex_data;
	int num_comp;
	stbi_uc* const image_data = stbi_load(data_asset_path.c_str(), &tex_data.width, &tex_data.height, &num_comp, 0);
	
	if (image_data == nullptr)
	{
		log::error("asset_loader", "wrong asset_type: (%s) %s", asset_type.second.c_str(), t_asset_path.c_str());
		return nullptr;
	}

	if (num_comp == 0)
	{
		log::error("asset_loader", "failed to load image, no valid color data: (%s) %s", asset_type.second.c_str(), t_asset_path.c_str());
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

std::shared_ptr<shader> asset_loader<shader>::load_asset(const std::string& t_asset_path)
{
	const std::string asset_path = paths::combine_paths(paths::content_asset_dir(), t_asset_path + ".json");
	const std::string asset_data_string = local::load_file_as_string(asset_path);
	if (asset_data_string.empty())
	{
		return nullptr;
	}

	const auto json_asset = json::parse(asset_data_string);

	const auto asset_type = local::try_parse_string(json_asset, "asset_type");
	if (!asset_type.first || asset_type.second != "shader")
	{
		log::error("asset_loader", "wrong asset_type: (%s) %s", asset_type.second.c_str(), t_asset_path.c_str());
		return nullptr;
	}

	const auto fragment_shader_asset = local::try_parse_string(json_asset, "fragment_shader");
	const std::string fragment_shader = local::load_file_as_string(paths::combine_paths(paths::data_asset_dir(), fragment_shader_asset.second));

	const auto vertex_shader_asset = local::try_parse_string(json_asset, "vertex_shader");
	const std::string vertex_shader = local::load_file_as_string(paths::combine_paths(paths::data_asset_dir(), vertex_shader_asset.second));

	return shader::create_shader(vertex_shader, fragment_shader);
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

	const auto asset_type = local::try_parse_string(json_asset, "asset_type");
	if (!asset_type.first || asset_type.second != "material")
	{
		log::error("asset_loader", "wrong asset_type: (%s) %s", asset_type.second.c_str(), t_asset_path.c_str());
		return nullptr;
	}

	const auto shader_ref = local::try_parse_string(json_asset, "shader");
	if (!shader_ref.first || shader_ref.second.empty())
	{
		log::error("asset_loader", "no shader specified for material: %s", t_asset_path.c_str());
		return nullptr;
	}

	std::shared_ptr<shader> shader_ptr = asset_loader<shader>::load_asset(shader_ref.second);

	const unform_values material_uniforms = [&]()->unform_values 
	{
		const auto asset_type_field = json_asset.find("uniforms");
		if (asset_type_field == json_asset.end())
			return {};

		const auto uniforms_obj = json_asset["uniforms"];

		unform_values out_uniforms;
		for (auto it = uniforms_obj.begin(); it != uniforms_obj.end(); ++it)
		{
			// try parse as float
			const auto float_field = local::try_parse_float(uniforms_obj, it.key());
			if (float_field.first)
			{
				log::info("asset_loader<material>", "Parsed uniform float: %f", float_field.second);
				out_uniforms.float_uniforms[it.key()] = float_field.second;
			}

			// try parse as 4d float array (vec4)
			const auto float_4d_field = local::try_parse_4d_float_array(uniforms_obj, it.key());
			if (float_4d_field.first)
			{
				log::info("asset_loader<material>", "Parsed uniform 4d float: %f %f %f %f", float_4d_field.second[0], float_4d_field.second[1], float_4d_field.second[2], float_4d_field.second[3]);
				out_uniforms.vector4_uniforms[it.key()] = { float_4d_field.second[0], float_4d_field.second[1], float_4d_field.second[2], float_4d_field.second[3] };
			}

			// try parse as 16d float array (matrix)
			const auto float_16d_field = local::try_parse_16d_float_array(uniforms_obj, it.key());
			if (float_16d_field.first)
			{
				log::info("asset_loader<material>", "Parsed uniform 16d float");
				out_uniforms.matrix4x4_uniforms[it.key()] = { 
					{ float_16d_field.second[0], float_16d_field.second[1], float_16d_field.second[2], float_16d_field.second[3] },
					{ float_16d_field.second[4], float_16d_field.second[5], float_16d_field.second[6], float_16d_field.second[7] }, 
					{ float_16d_field.second[8], float_16d_field.second[9], float_16d_field.second[10], float_16d_field.second[11] }, 
					{ float_16d_field.second[12], float_16d_field.second[13], float_16d_field.second[14], float_16d_field.second[15] },
				};
			}

			// try parse as string (texture ref)
			const auto string_field = local::try_parse_string(uniforms_obj, it.key());
			if (string_field.first)
			{
				log::info("asset_loader<material>", "Parsed uniform string", string_field.second.c_str());
				out_uniforms.texture_uniforms[it.key()] = asset_loader<texture>::load_asset(string_field.second);
			}
		}

		return out_uniforms;
	}();

	return material::create_material(shader_ptr, material_uniforms);
}