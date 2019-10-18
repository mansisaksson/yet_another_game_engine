#pragma once
#include <string>
#include <cstdarg>

struct paths
{
	static std::string content_dir()
	{
		return ENGINE_CONTENT_DIR;
	}

	static std::string combine_paths(const char* fmt...)
	{
		// TODO
	}
};