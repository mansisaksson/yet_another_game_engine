#pragma once

#include "gameframework/entity.h"

class static_mesh;

class player_entity : public entity
{
private:
	transform m_transform;
	std::shared_ptr<static_mesh> m_static_mesh_ptr;

	vector2 m_movement_input;
	float m_movement_speed = 2.f;

public:

	virtual void begin_play() override;
	virtual void tick(float delta_time) override;
	virtual void draw(const viewport* viewport) override;

public:

	void set_movement_vector(const vector2& new_mvn_vector);

};