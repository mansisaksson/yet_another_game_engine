#include "assetloaders.h"
#include "loaders/obj_loader.h"
#include <iostream>
#include <filesystem>
#include "../core/paths.h"

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