#pragma once

#include "tavla.h"

class tavla_window : public tavla
{
public:
	int width;
	int height;

public:
	tavla_window()
		: height(640)
		, width(480)
	{
	}

	virtual void tick(float t_delta_time) override
	{

	}

};