#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "assetloaders.h"

class asset_manager
{
private:

    std::unordered_map<std::string, std::weak_ptr<void>> m_asset_table;

private:

	asset_manager() {};
    asset_manager(const asset_manager&) = delete;
    asset_manager(asset_manager&&) = delete;
	void operator=(const asset_manager&) = delete;

public:

	static asset_manager& get()
	{
		static asset_manager m_asset_manager_instance;
		return m_asset_manager_instance;
	}

    template<class T>
    std::shared_ptr<T> load_asset(const std::string &t_asset_path)
    {
        const auto asset_ptr = m_asset_table.find(t_asset_path);
        if (asset_ptr != m_asset_table.end() && !(*asset_ptr).second.expired())
        {
			// TODO: since we're not using polymorfism, we cannot ensure that we're returning the proper type here.
            return std::static_pointer_cast<T>((*asset_ptr).second.lock());
        }

        const std::shared_ptr<T> loaded_asset = asset_loader<T>::load_asset(t_asset_path);

		if (loaded_asset)
		{
			m_asset_table.insert(std::make_pair(t_asset_path, loaded_asset));
			return loaded_asset->shared_from_this();
		}

		return nullptr;
    }
    
};
