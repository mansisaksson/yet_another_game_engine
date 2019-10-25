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
	vector3 m_camera_speed;
	float m_camera_movement_speed = 0.1f;
	
	viewport* m_viewport;
	vector3 m_view_location;
	quaternion m_view_rotation;

public:

	void start_game(scene* const game_scene, tavla_viewport* const game_viewport)
	{
		m_viewport = game_viewport->get_viewport();

		game_scene->spawn_entity<entity>(transform(vector3(10, 5, 0), quaternion(), 1.f));
	}

	void tick_game(float t_delta_time)
	{
		m_view_location += m_view_rotation.rotate_vector(m_camera_speed * t_delta_time);

		m_viewport->set_view_location(m_view_location);
	}

	void on_input_event(const input_event& t_input_event)
	{
		if (t_input_event.key == key::w)
		{
			m_camera_speed.x = (t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
		}
		if (t_input_event.key == key::s)
		{
			m_camera_speed.x = -(t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
		}


		if (t_input_event.key == key::d)
		{
			m_camera_speed.y = (t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
		}
		if (t_input_event.key == key::a)
		{
			m_camera_speed.y = -(t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
		}


		if (t_input_event.key == key::space)
		{
			m_camera_speed.z = (t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
		}
		if (t_input_event.key == key::left_control)
		{
			m_camera_speed.z = -(t_input_event.key_event != key_event::released ? m_camera_movement_speed : 0.f);
		}

		log::info("game", "%s : %s", key_to_string(t_input_event.key).c_str(), key_event_to_string(t_input_event.key_event).c_str());
	}
};