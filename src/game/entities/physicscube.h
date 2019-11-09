#pragma once

#include "gameframework/entity.h"

class static_mesh;
class rigid_body;

class physics_cube_entity : public entity
{
private:
	std::shared_ptr<static_mesh> m_static_mesh_ptr;
	std::shared_ptr<rigid_body> m_rigid_body;

	bool m_simulate_physics;
	float m_mass;

public:

	physics_cube_entity(bool simulate_physics, float mass)
		: m_simulate_physics(simulate_physics)
		, m_mass(mass)
	{}

	virtual void begin_play() override;
	virtual void removed_from_scene(scene* scene) override;
	virtual void tick(float delta_time) override;
	virtual void post_physics_tick() override;
	virtual void draw(const viewport* viewport) override;

};