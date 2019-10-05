#pragma once
#include <memory>

class platform_window;
class platform_input;

class platform
{
private:
	virtual std::shared_ptr<platform_window> create_platform_window() = 0;
	virtual std::shared_ptr<platform_input> create_platform_input() = 0;

public:
	static std::shared_ptr<platform> get_platform();
	static std::shared_ptr<platform_window> create_window();
	static std::shared_ptr<platform_input> get_platform_input();
};