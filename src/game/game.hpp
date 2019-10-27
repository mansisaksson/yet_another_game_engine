#pragma once
#include "core/core.h"
#include "gameframework/scene.h"
#include "gameframework/entity.h"
#include "platform/platforminput.h"
#include "rendercore/viewport.h"

class tavla_viewport;
class scene;

class game
{
private:
	vector3 m_camera_movement;
	float m_camera_movement_speed = 1.f;

	vector2 m_camera_rot;
	float m_camera_rot_speed = 0.01f;

	viewport* m_viewport;
	vector3 m_view_location;
	quaternion m_view_rotation;

public:

	void start_game(scene* const game_scene, tavla_viewport* const game_viewport)
	{
		m_viewport = game_viewport->get_viewport();

		game_scene->spawn_entity<entity>(transform(vector3(0, 0, 0), quaternion::identity, 1.f));
	}

	void tick_game(float t_delta_time)
	{
		m_view_rotation = quaternion(vector3(0, 1, 0), m_camera_rot.x) * m_view_rotation;
		//m_view_rotation = quaternion(vector3(1, 0, 0), m_camera_rot.y) * m_view_rotation;

		//log::info("test", "forward: %s", m_view_rotation.get_forward().to_string().c_str());
		m_view_location += m_view_rotation.rotate_vector(m_camera_movement * t_delta_time);

		m_viewport->set_view_location(m_view_location);
		m_viewport->set_view_rotation(m_view_rotation);
	}

	void on_input_event(const input_event& t_input_event)
	{
		// Move camera
		{
			if (t_input_event.key == key::w)
			{
				m_camera_movement.z = -(t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}
			if (t_input_event.key == key::s)
			{
				m_camera_movement.z = (t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}


			if (t_input_event.key == key::d)
			{
				m_camera_movement.x = -(t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}
			if (t_input_event.key == key::a)
			{
				m_camera_movement.x = (t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}


			if (t_input_event.key == key::space)
			{
				m_camera_movement.y = (t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
			}
			if (t_input_event.key == key::left_control)
			{
				m_camera_movement.y = -(t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
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