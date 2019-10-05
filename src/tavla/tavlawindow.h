#pragma once

#include "tavla.h"

class platform_window;

class tavla_window : public tavla
{
public:
	int width;
	int height;
	std::shared_ptr<platform_window> window;

public:
	tavla_window();
	~tavla_window();

	virtual void tick(float t_delta_time) override;
};