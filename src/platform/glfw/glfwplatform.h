#pragma once
#include <memory>
#include "platform/platform.h"

class glfw_platform : public platform
{
public:
	glfw_platform();
	~glfw_platform();

private:
	virtual std::shared_ptr<platform_window> create_platform_window() override;
	virtual std::shared_ptr<platform_input> create_platform_input() override;
	virtual float get_platform_time() const override;
};