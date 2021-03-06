#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <any>
#include "assetloaders.h"

class asset_manager
{
private:

    std::unordered_map<std::string, std::any> m_asset_table;

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

        if (asset_ptr != m_asset_table.end())
        {
			// TODO: this will throw if we encounter an incorrect type. This is however better than it just silently stepping through.
			const std::weak_ptr<T> casted_ptr = std::any_cast<std::weak_ptr<T>>((*asset_ptr).second);
			if (!casted_ptr.expired())
			{
				return casted_ptr.lock();
			}
        }

        const std::shared_ptr<T> loaded_asset = asset_loader<T>::load_asset(t_asset_path);

		if (loaded_asset)
		{
			m_asset_table[t_asset_path] = std::weak_ptr<T>(loaded_asset);
			return loaded_asset;
		}

		return nullptr;
    }
    
};
