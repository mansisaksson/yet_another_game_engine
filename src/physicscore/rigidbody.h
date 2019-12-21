#pragma once
#include "core/core.h"
#include "physicstypes.h"

class collision_shape;
class box_shape;
class sphere_shape;
class capsule_shape;

class rigid_body
{
private:
	friend class physics_scene;
	friend struct bt_yete_motion_state;

	class btRigidBody* m_bt_rigid_body;
	class btCompoundShape* m_bt_collision_shape;
	struct bt_yete_motion_state* m_bt_motion_state;
	std::vector<class btCollisionShape*> m_child_shapes;

private:
	collision_type m_collision_type = collision_type::kinematic;
	
	uint32_t m_blocking_group = (uint32_t)collision_channel::channel_1;
	uint32_t m_blocking_mask = (uint32_t)collision_channel::channel_1;

	uint32_t m_overlap_group = 0;
	uint32_t m_overlap_mask = 0;

	float m_mass = 100.f;
	float m_friction = 0.5f;
	float m_linear_damping = 0.1f;
	float m_angular_damping = 0.01f;
	float m_rolling_friction = 0.0f;
	float m_spinning_friction = 0.0f;		// torsional friction around contact normal
	float m_restitution = 0.0f;				// best simulation results using zero restitution.
	float m_linear_sleeping_threshold = 0.8f;
	float m_angular_sleeping_threshold = 1.0f;
	vector3 m_center_of_mass_offset;

	transform* m_shared_transform = nullptr;

	std::set<rigid_body*> m_overlapping_bodies;

public:

	rigid_body(transform* t_transform = nullptr, const vector3 &t_com_offset = vector3::zero);
	virtual ~rigid_body();

	void set_collision_type(collision_type t_collision_type);
	void set_collision_response_to_channel(collision_channel channel, collision_response response);
	void set_collision_channel(collision_channel channel);

	void set_mass(float t_mass);
	void set_friction(float t_friction);
	void set_linear_damping(float t_linear_damping);
	void set_angular_damping(float t_angular_damping);
	void set_rolling_friction(float t_rolling_friction);
	void set_spinning_friction(float t_spinning_friction);
	void set_restitution(float t_restitution);
	void set_linear_sleeping_threshold(float t_linear_sleeping_threshold);
	void set_angular_sleeping_threshold(float t_angular_sleeping_threshold);

	void set_world_location(const vector3& t_location);
	void set_world_rotation(const quaternion& t_rotation);
	void set_world_transform(const transform& t_transform);

	void set_center_of_mass_offset(const vector3& t_com_offset);

	void add_box_shape(const box_shape& box, const transform& shape_transform);
	void add_capsue_shape(const capsule_shape& capsule, const transform& shape_transform);
	void add_sphere_shape(const sphere_shape& sphere, const transform& shape_transform);

	float get_simulating_physics() const;
	float get_mass() const;
	float get_friction() const;
	float get_linear_damping() const;
	float get_angular_damping() const;
	float get_rolling_friction() const;
	float get_spinning_friction() const;
	float get_restitution() const;
	float get_linear_sleeping_threshold() const;
	float get_angular_sleeping_threshold() const;

	vector3 get_rigid_body_location() const;
	quaternion get_rigid_body_rotation() const;
	transform get_rigid_body_transform() const;

	vector3 get_center_of_mass_offset() const;

	vector3 get_linear_velocity() const;
	vector3 get_angular_velocity() const;
	vector3 get_velocity_at_point(const vector3& t_location) const;

	vector3 world_to_relative_loc(const vector3& t_location) const;

	void add_force(const vector3& t_force);
	void add_force_at_location(const vector3& t_force, const vector3 &t_location);

	void add_impulse(const vector3& t_impulse);
	void add_impulse_at_location(const vector3& t_impulse, const vector3& t_location);

	void add_torque(const vector3& t_torque);
	void add_torque_impulse(const vector3& t_torque_impulse);

	void set_linear_velocity(const vector3& t_linear_velocity);
	void set_angular_velocity(const vector3& t_angular_velocity);

	typedef delegate<void, const vector3&, const quaternion&> sync_transform_delegate;
	multicast_delegate<const vector3&, const quaternion&> on_synchronize_transform;

	typedef delegate<void, const std::vector<hit_result>&> on_hit_delegate;
	multicast_delegate<const std::vector<hit_result>&> on_hit;
	
	typedef delegate<void, rigid_body&> on_overlap_delegate;
	multicast_delegate<rigid_body&> on_begin_overlap;
	multicast_delegate<rigid_body&> on_end_overlap;

private:
	static rigid_body* bt_to_yete_rigid_body(const class btRigidBody* bt_rigid_body);

};