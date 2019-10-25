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
	inline std::weak_ptr<T> spawn_entity(const transform &t_transform, Params&... args)
	{
		const auto entity_ptr = std::make_shared<T>(args ...);
		entity_ptr->transform = t_transform;
		entities.push_back(entity_ptr);
		return std::weak_ptr<T>(entity_ptr);
	}

	void draw_scene(const viewport* viewport);
};