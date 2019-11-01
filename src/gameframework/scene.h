#pragma once
#include "core/core.h"

class entity;
class viewport;

class scene
{
private:
	std::vector<std::shared_ptr<entity>> entities;

public:
	template<class T, class... Params>
	inline std::weak_ptr<T> spawn_entity(Params&... args)
	{
		const auto entity_ptr = std::make_shared<T>(args ...);
		entities.push_back(entity_ptr);
		entity_ptr->begin_play();
		return std::weak_ptr<T>(entity_ptr);
	}

	void tick_scene(float t_delta_time);
	void draw_scene(const viewport* viewport);
};