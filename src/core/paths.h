#pragma once
#include <string>
#include <cstdarg>

struct paths
{
	static std::string content_dir()
	{
		// TODO: Set this in CMake
		return "C:/Users/mans_/Documents/GitHub/ordinary_cubes/content/";
	}

	static std::string combine_paths(const char* fmt...)
	{
		// TODO
	}
};