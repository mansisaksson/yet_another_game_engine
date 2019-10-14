#pragma once
#include "assetmanager.h"
#include "../rendercore/staticmesh.h"

template<class T>
class asset_loader
{
private:
	

public:
	static std::shared_ptr<T> load_asset(const std::string& t_asset_path);
};

std::shared_ptr<static_mesh> asset_loader<static_mesh>::load_asset(const std::string& t_asset_path);