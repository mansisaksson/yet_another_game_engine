#pragma once

#include "tavla.h"
#include "assetmanager/assetptr.h"

class platform_window;

enum class slot_h_alignment
{
	fill,
	left,
	right,
	center
};

enum class slot_v_alignment
{
	fill,
	top,
	bottom,
	center
};

class tavla_window : public tavla_base<tavla_window>
{
public:
	class slot : public tavla_base::slot<tavla_window::slot>
	{
	public:
		slot_h_alignment h_alignment = slot_h_alignment::fill;
		slot_v_alignment v_alignment = slot_v_alignment::fill;

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
	virtual void draw() override;
	virtual void post_draw() override;
	// ~end tavla interface

	std::tuple<int, int> get_window_size() const;

	bool has_valid_context() const;

	static std::weak_ptr<tavla_window> find_parent_window(const std::weak_ptr<tavla>& t_tavla);
};