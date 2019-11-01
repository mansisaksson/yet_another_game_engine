#pragma once
#include "core/core.h"

class viewport;

class entity
{
public:
	virtual void begin_play() {}
	virtual void tick(float delta_time) {}
	virtual void draw(const viewport* viewport) {}
};
