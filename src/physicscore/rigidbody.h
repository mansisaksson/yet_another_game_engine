#pragma once
#include "core/core.h"

class collision_shape;
class box_shape;
class sphere_shape;
class capsule_shape;

class rigid_body
{
private:
	friend class physics_scene;

	class btRigidBody* m_bt_rigid_body;
	class btCompoundShape* m_bt_collision_shape;
	std::vector<class btCollisionShape*> m_child_shapes;

private:
	bool m_simulate_physics = true;
	float m_mass = 100.f;
	float m_friction = 0.5f;
	float m_linear_damping = 0.1f;
	float m_angular_damping = 0.01f;
	float m_rolling_friction = 0.0f;
	float m_spinning_friction = 0.0f;		// torsional friction around contact normal
	float m_restitution = 0.0f;			// best simulation results using zero restitution.
	float m_linear_sleeping_threshold = 0.8f;
	float m_angular_sleeping_threshold = 1.0f;

public:

	rigid_body();
	virtual ~rigid_body();

	void set_simulate_physics(bool t_simulate_physics);
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

	void add_box_shape(const box_shape& box, const transform& shape_transform);
	void add_capsue_shape(const capsule_shape& capsule, const transform& shape_transform);
	void add_sphere_shape(const sphere_shape& sphere, const transform& shape_transform);

	vector3 get_rigid_body_location() const;
	quaternion get_rigid_body_rotation() const;

};