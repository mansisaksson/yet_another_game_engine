#include "rigidbody.h"
#include "collisionshapes/collisionshapes.h"
#include "bt_helpers.h"

#include <bullet/btBulletDynamicsCommon.h>

rigid_body::rigid_body()
	: rigid_body(rigid_body::construction_info())
{
}

rigid_body::rigid_body(const construction_info& t_construction_info)
{
	m_bt_collision_shape = new btCompoundShape(true, (int)t_construction_info.collision_shapes.size());
	m_child_shapes.reserve(t_construction_info.collision_shapes.size());

	for (const auto &shape_pair : t_construction_info.collision_shapes)
	{
		const collision_shape* yete_shape = shape_pair.shape;
		const transform &shape_transform = shape_pair.transform;

		btCollisionShape* const bt_collision_shape = [&]()->btCollisionShape*
		{
			if (yete_shape->is_box())
			{
				const box_shape* box = static_cast<const box_shape*>(yete_shape);
				return new btBoxShape(bt_helpers::yete_to_bt_vector3_abs(box->get_box_half_extent()));
			}
			else if (yete_shape->is_sphere())
			{

			}
			else if (yete_shape->is_capsule())
			{

			}

			return nullptr;
		}();

		if (bt_collision_shape)
		{
			m_child_shapes.push_back(bt_collision_shape);
			m_bt_collision_shape->addChildShape(bt_helpers::yete_to_bt_transform(shape_transform), bt_collision_shape);
			//m_bt_collision_shape = bt_collision_shape;
		}
		else
		{
			log::error("rigid_body", "Invalid physics shape added to rigid body");
		}
	}

	const bool is_dynamic = t_construction_info.simulate_physics;
	btVector3 local_inertia(0, 0, 0);
	if (is_dynamic)
		m_bt_collision_shape->calculateLocalInertia(t_construction_info.mass, local_inertia);

	btRigidBody::btRigidBodyConstructionInfo bt_construction_info(is_dynamic ? t_construction_info.mass : 0.f, nullptr, m_bt_collision_shape, local_inertia);
	bt_construction_info.m_startWorldTransform = bt_helpers::yete_to_bt_transform(t_construction_info.start_world_transform);
	bt_construction_info.m_linearDamping = t_construction_info.linear_damping;
	bt_construction_info.m_angularDamping = t_construction_info.angular_damping;
	bt_construction_info.m_friction = t_construction_info.friction;
	bt_construction_info.m_rollingFriction = t_construction_info.rolling_friction;
	bt_construction_info.m_spinningFriction = t_construction_info.spinning_friction;
	bt_construction_info.m_restitution = t_construction_info.restitution;
	bt_construction_info.m_linearSleepingThreshold = t_construction_info.linear_sleeping_threshold;
	bt_construction_info.m_angularSleepingThreshold = t_construction_info.angular_sleeping_threshold;

	m_bt_rigid_body = new btRigidBody(bt_construction_info);
	m_bt_rigid_body->setUserIndex(-1);
}

rigid_body::~rigid_body()
{
	// TODO: What cleanup is needed by bullet?

	for (auto* shape : m_child_shapes)
		delete shape;
	
	delete m_bt_collision_shape;
	delete m_bt_rigid_body;
}

vector3 rigid_body::get_rigid_body_location() const
{
	return bt_helpers::bt_to_yete_vector3(m_bt_rigid_body->getWorldTransform().getOrigin());
}

quaternion rigid_body::get_rigid_body_rotation() const
{
	return bt_helpers::bt_to_yete_quaternion(m_bt_rigid_body->getWorldTransform().getRotation());
}