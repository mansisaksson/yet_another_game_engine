#pragma once
#include <string>

struct paths
{
	static std::string content_dir()
	{
		return std::string(ENGINE_CONTENT_DIR);
	}

	static std::string content_asset_dir()
	{
		return std::string(ENGINE_CONTENT_DIR) + "/assets";
	}

	static std::string data_asset_dir()
	{
		return std::string(ENGINE_CONTENT_DIR) + "/data";
	}

	template<class ... Args>
	static std::string combine_paths(Args... args)
	{
		const auto combile_paths_internal = [](const std::initializer_list<std::string> &l)
		{
			int count = 0;
			std::string final_str;
			for (auto str : l)
			{
				if (count != 0)
				{
					if (str.find_first_of("/") == 0)
					{
						str.erase(0, 1);
					}
				}

				if (str.find_last_of("/") == final_str.length() - 1)
				{
					str.erase(final_str.length() - 1);
				}

				if (count == 0)
					final_str = str;
				else
					final_str += "/" + str;

				++count;
			}

			return final_str;
		};

		return combile_paths_internal({ args... });
	}
};