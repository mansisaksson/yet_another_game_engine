#pragma once

#include "tavla/tavla.h"

class horizontal_box : public tavla_base<horizontal_box>
{
public:
	class slot : public tavla_base::slot<horizontal_box::slot>
	{
	public:
		slot(const std::shared_ptr<horizontal_box> & t_parent)
			: tavla_base::slot<horizontal_box::slot>(t_parent)
		{}
	};

	std::shared_ptr<horizontal_box::slot> add_slot()
	{
		return add_slot_of_type<horizontal_box::slot>();
	}

public:
	// ~begin tavla interface
	virtual dimensions calculate_slot_dimentions(const std::shared_ptr<tavla::slot>& slot) const override;
	// ~end tavla interface

};