#pragma once

#include "tavla.h"
#include "../core/core.h"

class tavla_application : public tavla
{
public:
	class slot : public tavla::slot
	{
	public:
		slot(std::shared_ptr<tavla_application> t_application)
			: tavla::slot(t_application)
		{}

		std::shared_ptr<tavla_application> set_application_content(std::shared_ptr<tavla> t_content)
		{
			return set_content<tavla_application>(t_content);
		}
	};

	std::shared_ptr<tavla_application::slot> add_application_slot()
	{
		return add_slot<tavla_application, tavla_application::slot>();
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