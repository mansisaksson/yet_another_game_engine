#pragma once
#include "core/core.h"
#include "gameframework/scene.h"
#include "gameframework/entity.h"
#include "platform/platforminput.h"
#include "rendercore/viewport.h"

#include "entities/physicscube.h"

class tavla_viewport;

class physics_game
{
private:
	vector3 m_camera_movement;
	float m_camera_movement_speed = 5.f;

	vector2 m_camera_rot;
	float m_camera_rot_speed = 1.f;

	viewport* m_viewport;
	vector3 m_view_location = vector3(-12, 0, 0);
	vector2 m_view_rotation;

	scene* game_scene;

public:

	void start_game(scene* const t_game_scene, tavla_viewport* const game_viewport)
	{
		game_scene = t_game_scene;
		m_viewport = game_viewport->get_viewport();

		
		game_scene->spawn_entity<physics_cube_entity>(transform(vector3(0, 0, -35)), 20.f, false, 1.f);

		const int num_cubes = 4;
		for (int z = 0; z < num_cubes; z++)
		{
			for (int y = 0; y < num_cubes; y++)
			{
				for (int x = 0; x < num_cubes; x++)
				{
					const int cube_size = 1.f;
					const auto cube_location = vector3(cube_size * x, cube_size * y, cube_size * z);

					game_scene->spawn_entity<physics_cube_entity>(cube_location, cube_size / 2.f, true, 1.f);
				}
			}
		}
	}

	void tick_game(float t_delta_time)
	{
		game_scene->tick_scene(t_delta_time);

		m_view_rotation += m_camera_rot * t_delta_time;

		const quaternion new_view_rotation = quaternion(vector3::up, m_view_rotation.x) * quaternion(vector3::right, -m_view_rotation.y);

		if (m_camera_movement.length() > 0 || m_camera_rot.length() > 0)
		{
			m_view_location += new_view_rotation.rotate_vector(m_camera_movement * t_delta_time);
			//log::info("test", "camera location: %s", m_view_location.to_string().c_str());
			//log::info("test", "camera forward: %s", new_view_rotation.get_forward().to_string().c_str());
			//log::info("test", "m_view_rotation: %s", m_view_rotation.to_string().c_str());
		}

		m_viewport->set_view_location(m_view_location);
		m_viewport->set_view_rotation(new_view_rotation);
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