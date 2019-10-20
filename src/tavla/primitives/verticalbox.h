#pragma once

#include "tavla/tavla.h"

class vertical_box : public tavla_base<vertical_box>
{
public:
	class slot : public tavla_base::slot<vertical_box::slot>
	{
	public:
		slot(const std::shared_ptr<vertical_box> & t_parent)
			: tavla_base::slot<vertical_box::slot>(t_parent)
		{}
	};

	std::shared_ptr<vertical_box::slot> add_slot()
	{
		return add_slot_of_type<vertical_box::slot>();
	}

public:
	// ~begin tavla interface
	virtual dimensions calculate_slot_dimentions(const std::shared_ptr<tavla::slot>& slot) const override;
	// ~end tavla interface

};