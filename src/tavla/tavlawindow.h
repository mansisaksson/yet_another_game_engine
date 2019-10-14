#pragma once

#include "tavla.h"

class platform_window;

class tavla_window : public tavla_base<tavla_window>
{
public:
	class slot : public tavla_base::slot<tavla_window::slot>
	{
	public:
		slot(const std::shared_ptr<tavla_window> & t_parent)
			: tavla_base::slot<tavla_window::slot>(t_parent)
		{}
	};

	std::shared_ptr<tavla_window::slot> add_slot()
	{
		return add_slot_of_type<tavla_window::slot>();
	}

public:
	int width;
	int height;
	std::string title;

private:
	std::shared_ptr<platform_window> m_window;

public:
	// ~begin tavla interface
	virtual void construct() override;
	virtual void destruct() override;
	virtual void tick(float t_delta_time) override;
	// ~end tavla interface
};