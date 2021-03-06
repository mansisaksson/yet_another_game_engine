#include "horizontalbox.h"
#include <algorithm>

dimensions horizontal_box::calculate_slot_dimentions(const std::shared_ptr<tavla::slot>& slot) const
{
	dimensions out_dimensions;

	const auto tavla_dimensions = get_tavla_dimentions();
	const int slot_index = (int)std::distance(m_child_slots.begin(), std::find(m_child_slots.begin(), m_child_slots.end(), slot));

	out_dimensions.width = tavla_dimensions.width / (int)m_child_slots.size();
	out_dimensions.height = tavla_dimensions.height;
	
	out_dimensions.x_pos = tavla_dimensions.x_pos + (out_dimensions.width * slot_index);
	out_dimensions.y_pos = tavla_dimensions.y_pos;

	return out_dimensions;
}