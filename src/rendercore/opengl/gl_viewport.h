#pragma once
#include "rendercore/viewport.h"

class gl_viewport : public viewport
{
public:
	virtual void make_current(int x, int y, int width, int height, int window_width, int widow_height) override;
};