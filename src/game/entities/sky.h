#pragma once

#include "gameframework/entity.h"

class static_mesh;

class sky_entity : public entity
{
private:
	std::shared_ptr<static_mesh> m_static_mesh_ptr;

public:

	virtual void begin_play() override;
	virtual void draw(const viewport* viewport) override;

public:

};