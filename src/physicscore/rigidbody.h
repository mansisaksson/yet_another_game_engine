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
	//class btCollisionShape* m_bt_collision_shape;
	std::vector<class btCollisionShape*> m_child_shapes;

public:

	rigid_body();

	struct rb_collision
	{
		const collision_shape* shape;
		transform transform;
	};

	struct construction_info
	{
		bool simulate_physics	= true;
		float mass				= 100.f;
		float friction			= 0.5f;
		float linear_damping	= 0.1f;
		float angular_damping	= 0.01f;
		float rolling_friction	= 0.0f;
		float spinning_friction	= 0.0f;		//torsional friction around contact normal
		float restitution		= 0.0f;		//best simulation results using zero restitution.
		float linear_sleeping_threshold		= 0.8f;
		float angular_sleeping_threshold	= 1.0f;

		transform start_world_transform;
		std::vector<rb_collision> collision_shapes;
	};

	rigid_body(const construction_info& t_construction_info);

	virtual ~rigid_body();

	/*void add_box_shape(const box_shape& box, const transform& shape_transform);

	void add_capsue_shape(const box_shape& box, const transform& shape_transform);

	void add_sphere_shape(const box_shape& box, const transform& shape_transform);*/

	vector3 get_rigid_body_location() const;

	quaternion get_rigid_body_rotation() const;

};