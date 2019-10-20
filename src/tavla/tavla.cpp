#include "tavla.h"

dimensions tavla::get_tavla_dimentions() const
{ 
	return !m_parent_slot.expired() ? m_parent_slot.lock()->get_dimensions() : dimensions(); 
}

void tavla::tick_tavla_tree(const std::shared_ptr<tavla>& t_root_tavla, const float t_delta_time)
{
	traverse_tree(t_root_tavla, [t_delta_time](const std::shared_ptr<tavla>& t_tavla) {
		if (t_tavla->m_constructed)
			t_tavla->tick(t_delta_time);
	});
}

void tavla::draw_tavla_tree(const std::shared_ptr<tavla>& t_root_tavla)
{
	traverse_tree(t_root_tavla, [](const std::shared_ptr<tavla>& t_tavla) {
		if (t_tavla->m_constructed)
			t_tavla->draw();
	});

	traverse_tree(t_root_tavla, [](const std::shared_ptr<tavla>& t_tavla) {
		if (t_tavla->m_constructed)
			t_tavla->post_draw();
	});
}

void tavla::construct_tavla_tree(const std::shared_ptr<tavla>& t_root_tavla)
{
	traverse_tree(t_root_tavla, [](const std::shared_ptr<tavla>& t_tavla) {
		if (!t_tavla->m_constructed)
		{
			t_tavla->construct();
			t_tavla->m_constructed = true;
		}
	});
}

void tavla::calculate_tavla_tree_dimensions(const std::shared_ptr<tavla>& t_root_tavla)
{
	traverse_tree(t_root_tavla, [](const std::shared_ptr<tavla>& t_tavla) {
		if (t_tavla->m_constructed)
		{
			for (const auto slot : t_tavla->m_child_slots)
			{
				if (slot)
					slot->m_dimensions = t_tavla->calculate_slot_dimentions(slot);
			}
		}
	});
}

void tavla::traverse_tree(const std::shared_ptr<tavla>& t_root_tavla, const std::function<void(const std::shared_ptr<tavla>&)>& callback)
{
	struct local
	{
		static void get_tavla_recursive(const std::shared_ptr<tavla::slot>& t_slot, const std::function<void(const std::shared_ptr<tavla>&)>& callback)
		{
			const auto slot_content = t_slot ? t_slot->get_content() : nullptr;
			if (slot_content)
			{
				callback(slot_content);
				for (const auto& child_slot : slot_content->get_child_slots())
				{
					if (child_slot)
						get_tavla_recursive(child_slot, callback);
				}
			}
		}
	};

	callback(t_root_tavla);

	for (const auto& slot : t_root_tavla->get_child_slots())
	{
		local::get_tavla_recursive(slot, callback);
	}
}