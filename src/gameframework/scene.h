#pragma once
#include "core/core.h"
#include "entity.h"

class scene
{
	std::vector<std::shared_ptr<entity>> entities;

	void draw_scene();
};