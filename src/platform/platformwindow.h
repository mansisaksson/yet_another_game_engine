#pragma once
#include "core/core.h"

class platform_window : public std::enable_shared_from_this<platform_window>
{
public:
	multicast_delegate<platform_window&> on_window_closed;

protected:
	platform_window() = default;
	platform_window(const platform_window&) = delete;
	platform_window(platform_window&&) = delete;
	platform_window(platform_window* const) = delete;
	
public:
	virtual void init(int t_width, int t_height, std::string t_title) = 0;

	virtual void make_current() = 0;

	virtual void clear(const color &clear_color) = 0;

	virtual void close_window() = 0;

	virtual void swap_buffers() = 0;

	virtual std::tuple<int, int> get_window_size() const = 0;

};