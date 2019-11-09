#pragma once
#include "core/core.h"

class entity;
class viewport;
class physics_scene;

class scene
{
private:
	std::shared_ptr<physics_scene> m_physics_scene;
	std::vector<std::shared_ptr<entity>> m_entities;

public:
	scene();

	template<class T, class... Params>
	inline std::weak_ptr<T> spawn_entity(const transform &transform, Params... args)
	{
		const auto entity_ptr = std::make_shared<T>(args ...);
		add_entity(transform, entity_ptr);
		return std::weak_ptr<T>(entity_ptr);
	}

	void add_entity(const transform & transform, const std::shared_ptr<entity>& t_entity);
	void remove_entity(const entity* t_entity);

	void tick_scene(float t_delta_time);
	void draw_scene(const viewport* viewport);

	physics_scene* get_physics_scene() const;
};