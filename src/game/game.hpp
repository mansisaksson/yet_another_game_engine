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
	vector3 m_camera_movement;
	float m_camera_movement_speed = 1.f;

	vector2 m_camera_rot;
	float m_camera_rot_speed = 1.f;

	viewport* m_viewport;
	vector3 m_view_location;
	vector2 m_view_rotation;

	std::weak_ptr<player_entity> player;

public:

	void start_game(scene* const game_scene, tavla_viewport* const game_viewport)
	{
		m_viewport = game_viewport->get_viewport();

		m_viewport->set_view_location(vector3(0, 0, 5));
		m_viewport->set_view_rotation(matrix4x4::make_rot_matrix_from_xz(-vector3::up, vector3::forward).to_quaternion());

		player = game_scene->spawn_entity<player_entity>();
	}

	void tick_game(float t_delta_time)
	{
		m_view_rotation += m_camera_rot * t_delta_time;

		const quaternion new_view_rotation = quaternion(vector3::up, m_view_rotation.x) * quaternion(vector3::right, -m_view_rotation.y);

		if (m_camera_movement.length() > 0 || m_camera_rot.length() > 0)
		{
			m_view_location += new_view_rotation.rotate_vector(m_camera_movement * t_delta_time);
			//log::info("test", "camera location: %s", m_view_location.to_string().c_str());
			//log::info("test", "camera forward: %s", new_view_rotation.get_forward().to_string().c_str());
			//log::info("test", "m_view_rotation: %s", m_view_rotation.to_string().c_str());
		}

		/*m_viewport->set_view_location(m_view_location);
		m_viewport->set_view_rotation(new_view_rotation);*/
	}

	void on_input_event(const input_event& t_input_event)
	{
		// Move camera
		{
			if (t_input_event.key == key::w)
			{
				m_camera_movement.x = (t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}
			if (t_input_event.key == key::s)
			{
				m_camera_movement.x = -(t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}


			if (t_input_event.key == key::d)
			{
				m_camera_movement.y = (t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}
			if (t_input_event.key == key::a)
			{
				m_camera_movement.y = -(t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}


			if (t_input_event.key == key::space)
			{
				m_camera_movement.z = (t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}
			if (t_input_event.key == key::left_control)
			{
				m_camera_movement.z = -(t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}
		}

		// Rotate camea
		{
			if (t_input_event.key == key::right)
			{
				m_camera_rot.x = (t_input_event.key_event != key_event::released ? m_camera_rot_speed : 0.f);
			}
			if (t_input_event.key == key::left)
			{
				m_camera_rot.x = -(t_input_event.key_event != key_event::released ? m_camera_rot_speed : 0.f);
			}

			if (t_input_event.key == key::up)
			{
				m_camera_rot.y = (t_input_event.key_event != key_event::released ? m_camera_rot_speed : 0.f);
			}
			if (t_input_event.key == key::down)
			{
				m_camera_rot.y = -(t_input_event.key_event != key_event::released ? m_camera_rot_speed : 0.f);
			}
		}

		log::info("game", "%s : %s", key_to_string(t_input_event.key).c_str(), key_event_to_string(t_input_event.key_event).c_str());
	}
};