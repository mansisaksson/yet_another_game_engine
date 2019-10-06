#pragma once

#include "tavla.h"
#include "../core/core.h"

class tavla_application : public tavla_base<tavla_application>
{
public:
	class slot : public tavla_base::slot<tavla_application::slot>
	{
	public:
		slot(const std::shared_ptr<tavla_application> &t_parent)
			: tavla_base::slot<tavla_application::slot>(t_parent)
		{}
	};

	std::shared_ptr<tavla_application::slot> add_slot()
	{
		return add_slot_of_type<tavla_application::slot>();
	}

private:
	guid input_event_listen_id;

public:
	// ~begin tavla interface
	virtual void construct() override;
	virtual void destruct() override;
	virtual void tick(float t_delta_time) override;
	// ~end tavla interface

	int run();
};