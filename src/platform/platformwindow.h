#pragma once
#include <string>
#include <memory>

class platform_window : public std::enable_shared_from_this<platform_window>
{
protected:
	platform_window() = default;
	platform_window(const platform_window&) = delete;
	platform_window(platform_window&&) = delete;
	platform_window(platform_window* const) = delete;
	
public:
	virtual void init(int t_width, int t_height, std::string t_title) = 0;
	virtual void close_window() = 0;
};