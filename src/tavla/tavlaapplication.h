#pragma once

#include "tavla.h"

class tavla_application : public tavla
{
public:
	tavla_application();

	virtual void tick(float t_delta_time) override;

	int run();
};