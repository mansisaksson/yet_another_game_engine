#pragma once

#include "gameframework/entity.h"

class static_mesh;

class bullet_entity : public entity
{
private:
	std::shared_ptr<static_mesh> m_bullet_mesh;
	vector3 m_velocity;
	float m_lifetime;

public:
	bullet_entity(const vector3& velocity, float lifetime);

protected:

	virtual void begin_play() override;
	virtual void tick(float delta_time) override;
	virtual void draw(const viewport* viewport) override;

};
