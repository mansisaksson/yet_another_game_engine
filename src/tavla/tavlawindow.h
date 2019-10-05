#pragma once

#include "tavla.h"

class platform_window;

class tavla_window : public tavla
{
public:
	class slot : public tavla::slot
	{
	public:
		slot(std::shared_ptr<tavla_window> t_application)
			: tavla::slot(t_application)
		{}

		std::shared_ptr<tavla_window> set_window_content(std::shared_ptr<tavla> t_content)
		{
			return set_content<tavla_window>(t_content);
		}
	};

	std::shared_ptr<tavla_window::slot> add_window_slot()
	{
		return add_slot<tavla_window, tavla_window::slot>();
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