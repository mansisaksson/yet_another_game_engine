#pragma once
#include "core/core.h"
#include "gameframework/scene.h"
#include "gameframework/entity.h"
#include "platform/platforminput.h"
#include "rendercore/viewport.h"
#include "entities/player.h"

class tavla_viewport;

class game
{
private:
	vector2 m_movement_input;

	viewport* m_viewport;

	std::weak_ptr<player_entity> player;

public:

	void start_game(scene* const game_scene, tavla_viewport* const game_viewport)
	{
		m_viewport = game_viewport->get_viewport();

		m_viewport->set_view_location(vector3(0, 0, 10));
		m_viewport->set_view_rotation(matrix4x4::make_rot_matrix_from_xy(-vector3::up, vector3::right).to_quaternion());

		player = game_scene->spawn_entity<player_entity>(transform::identity);
	}

	void tick_game(float t_delta_time)
	{
		player.lock()->set_movement_vector(m_movement_input);
	}

	void on_input_event(const input_event& t_input_event)
	{
		if (t_input_event.key == key::w)
		{
			if (t_input_event.key_event == key_event::pressed)
			{
				m_movement_input.y += 1.f;
			}
			else if (t_input_event.key_event == key_event::released)
			{
				m_movement_input.y -= 1.f;
			}
		}

		if (t_input_event.key == key::s)
		{
			if (t_input_event.key_event == key_event::pressed)
			{
				m_movement_input.y -= 1.f;
			}
			else if (t_input_event.key_event == key_event::released)
			{
				m_movement_input.y += 1.f;
			}
		}


		if (t_input_event.key == key::d)
		{
			if (t_input_event.key_event == key_event::pressed)
			{
				m_movement_input.x += 1.f;
			}
			else if (t_input_event.key_event == key_event::released)
			{
				m_movement_input.x -= 1.f;
			}
		}

		if (t_input_event.key == key::a)
		{
			if (t_input_event.key_event == key_event::pressed)
			{
				m_movement_input.x -= 1.f;
			}
			else if (t_input_event.key_event == key_event::released)
			{
				m_movement_input.x += 1.f;
			}
		}

		if (t_input_event.key == key::space)
		{
			if (t_input_event.key_event == key_event::pressed)
			{
				player.lock()->fire();
			}
		}
	}
};