#pragma once
#include "core/core.h"

class viewport;
class scene;

class entity
{
private:
	friend class scene;

	scene* current_scene;

protected:
	transform transform;

public:
	virtual void begin_play() {}
	virtual void removed_from_scene(scene* scene) {}

	virtual void tick(float delta_time) {}
	virtual void post_physics_tick() {}
	virtual void draw(const viewport* viewport) {}

public:

	void remove_from_scene();

	inline scene* get_scene() const { return current_scene; }

};
