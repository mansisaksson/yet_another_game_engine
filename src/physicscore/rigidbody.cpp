#include "rigidbody.h"
#include "collisionshapes/collisionshapes.h"
#include "bt_helpers.h"

#include <bullet/btBulletDynamicsCommon.h>

rigid_body::rigid_body()
{
	m_bt_collision_shape = new btCompoundShape();

	btRigidBody::btRigidBodyConstructionInfo bt_construction_info(m_simulate_physics ? m_mass : 0.f, nullptr, m_bt_collision_shape);
	
	bt_construction_info.m_linearDamping = m_linear_damping;
	bt_construction_info.m_angularDamping = m_angular_damping;
	bt_construction_info.m_friction = m_friction;
	bt_construction_info.m_rollingFriction = m_rolling_friction;
	bt_construction_info.m_spinningFriction = m_spinning_friction;
	bt_construction_info.m_restitution = m_restitution;
	bt_construction_info.m_linearSleepingThreshold = m_linear_sleeping_threshold;
	bt_construction_info.m_angularSleepingThreshold = m_angular_sleeping_threshold;

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

void rigid_body::set_simulate_physics(bool t_simulate_physics)
{
	m_simulate_physics = t_simulate_physics;

	btVector3 local_inertia(0, 0, 0);
	if (m_simulate_physics)
		m_bt_collision_shape->calculateLocalInertia(m_mass, local_inertia);

	m_bt_rigid_body->setMassProps(m_simulate_physics ? m_mass : 0.f, local_inertia);
}

void rigid_body::set_mass(float t_mass)
{
	// TODO: Check for <= 0 mass

	m_mass = t_mass;

	btVector3 local_inertia(0, 0, 0);
	if (m_simulate_physics)
		m_bt_collision_shape->calculateLocalInertia(m_mass, local_inertia);

	m_bt_rigid_body->setMassProps(m_simulate_physics ? m_mass : 0.f, local_inertia);
}

void rigid_body::set_friction(float t_friction)
{
	m_friction = t_friction;
	m_bt_rigid_body->setFriction(m_friction);
}

void rigid_body::set_linear_damping(float t_linear_damping)
{
	m_linear_damping = t_linear_damping;
	m_bt_rigid_body->setDamping(m_linear_damping, m_angular_damping);
}

void rigid_body::set_angular_damping(float t_angular_damping)
{
	m_angular_damping = t_angular_damping;
	m_bt_rigid_body->setDamping(m_linear_damping, m_angular_damping);
}

void rigid_body::set_rolling_friction(float t_rolling_friction)
{
	m_rolling_friction = t_rolling_friction;
	m_bt_rigid_body->setRollingFriction(m_rolling_friction);
}

void rigid_body::set_spinning_friction(float t_spinning_friction)
{
	m_friction = t_spinning_friction;
	m_bt_rigid_body->setSpinningFriction(m_friction);
}

void rigid_body::set_restitution(float t_restitution)
{
	m_restitution = t_restitution;
	m_bt_rigid_body->setRestitution(m_restitution);
}

void rigid_body::set_linear_sleeping_threshold(float t_linear_sleeping_threshold)
{
	m_linear_sleeping_threshold = t_linear_sleeping_threshold;
	m_bt_rigid_body->setSleepingThresholds(m_linear_sleeping_threshold, m_angular_sleeping_threshold);
}

void rigid_body::set_angular_sleeping_threshold(float t_angular_sleeping_threshold)
{
	m_angular_sleeping_threshold = t_angular_sleeping_threshold;
	m_bt_rigid_body->setSleepingThresholds(m_linear_sleeping_threshold, m_angular_sleeping_threshold);
}

void rigid_body::set_world_location(const vector3& t_location)
{
	const auto current_transform = m_bt_rigid_body->getWorldTransform();
	m_bt_rigid_body->setWorldTransform(btTransform(current_transform.getRotation(), bt_helpers::yete_to_bt_vector3(t_location)));
}

void rigid_body::set_world_rotation(const quaternion& t_rotation)
{
	const auto current_transform = m_bt_rigid_body->getWorldTransform();
	m_bt_rigid_body->setWorldTransform(btTransform(bt_helpers::yete_to_bt_quaternion(t_rotation), current_transform.getOrigin()));
}

void rigid_body::add_box_shape(const box_shape& box, const transform& shape_transform)
{
	auto bt_box_shape = new btBoxShape(bt_helpers::yete_to_bt_vector3_abs(box.get_box_half_extent()));
	m_child_shapes.push_back(bt_box_shape);
	m_bt_collision_shape->addChildShape(bt_helpers::yete_to_bt_transform(shape_transform), bt_box_shape);

	m_bt_rigid_body->updateInertiaTensor();
}

void rigid_body::add_capsue_shape(const capsule_shape& capsule, const transform& shape_transform)
{
	auto bt_capsule_shape = new btCapsuleShape(capsule.get_radius(), capsule.get_half_height());
	m_child_shapes.push_back(bt_capsule_shape);
	m_bt_collision_shape->addChildShape(bt_helpers::yete_to_bt_transform(shape_transform), bt_capsule_shape);

	m_bt_rigid_body->updateInertiaTensor();
}

void rigid_body::add_sphere_shape(const sphere_shape& sphere, const transform& shape_transform)
{
	auto bt_sphere_shape = new btSphereShape(sphere.get_radius());
	m_child_shapes.push_back(bt_sphere_shape);
	m_bt_collision_shape->addChildShape(bt_helpers::yete_to_bt_transform(shape_transform), bt_sphere_shape);

	m_bt_rigid_body->updateInertiaTensor();
}

vector3 rigid_body::get_rigid_body_location() const
{
	return bt_helpers::bt_to_yete_vector3(m_bt_rigid_body->getWorldTransform().getOrigin());
}

quaternion rigid_body::get_rigid_body_rotation() const
{
	return bt_helpers::bt_to_yete_quaternion(m_bt_rigid_body->getWorldTransform().getRotation());
}