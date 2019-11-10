#include "rigidbody.h"
#include "collisionshapes/collisionshapes.h"
#include "bt_helpers.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/LinearMath/btMotionState.h>

ATTRIBUTE_ALIGNED16(struct)
bt_yete_motion_state : public btMotionState
{
	rigid_body* m_rigid_body;
	//void* m_userPointer;

	BT_DECLARE_ALIGNED_ALLOCATOR();

	bt_yete_motion_state(rigid_body* t_rigid_body)
		: m_rigid_body(t_rigid_body)
	{
	}

	// Synchronizes world transform from user to physics
	virtual void getWorldTransform(btTransform& centerOfMassWorldTrans) const
	{
		// TODO: Should check for scale changes here.

		const auto com_offset = bt_helpers::yete_to_bt_vector3(m_rigid_body->get_center_of_mass_offset());
		centerOfMassWorldTrans = m_rigid_body->m_bt_rigid_body->getWorldTransform() * btTransform(btQuaternion::getIdentity(), com_offset).inverse();
	}

	// Synchronizes world transform from physics to user
	// Bullet only calls the update of worldtransform for active objects
	virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
	{
		const auto com_offset_transform = btTransform(btQuaternion::getIdentity(), bt_helpers::yete_to_bt_vector3(m_rigid_body->get_center_of_mass_offset()));
		const auto graphics_world_trans = bt_helpers::bt_to_yete_transform(centerOfMassWorldTrans * com_offset_transform);
		m_rigid_body->on_synchronize_transform.broadcast(graphics_world_trans.location, graphics_world_trans.rotation);
	}
};

rigid_body::rigid_body(const transform& t_start_transform, const vector3& t_com_offset)
	: m_center_of_mass_offset(t_com_offset)
{
	m_bt_collision_shape = new btCompoundShape();

	btRigidBody::btRigidBodyConstructionInfo bt_construction_info(m_simulate_physics ? m_mass : 0.f, nullptr, m_bt_collision_shape);
	bt_construction_info.m_startWorldTransform			= bt_helpers::yete_to_bt_transform(t_start_transform);
	bt_construction_info.m_linearDamping				= m_linear_damping;
	bt_construction_info.m_angularDamping				= m_angular_damping;
	bt_construction_info.m_friction						= m_friction;
	bt_construction_info.m_rollingFriction				= m_rolling_friction;
	bt_construction_info.m_spinningFriction				= m_spinning_friction;
	bt_construction_info.m_restitution					= m_restitution;
	bt_construction_info.m_linearSleepingThreshold		= m_linear_sleeping_threshold;
	bt_construction_info.m_angularSleepingThreshold		= m_angular_sleeping_threshold;

	m_bt_rigid_body = new btRigidBody(bt_construction_info);
	m_bt_rigid_body->setUserIndex(-1);

	// Defer the creation of the motion state until we have a valid btRigidBody
	m_bt_motion_state = new bt_yete_motion_state(this);
	m_bt_rigid_body->setMotionState(m_bt_motion_state);
}

rigid_body::~rigid_body()
{
	// TODO: What cleanup is needed by bullet?

	for (auto* shape : m_child_shapes)
		delete shape;
	
	delete m_bt_collision_shape;
	delete m_bt_motion_state;
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
	//m_bt_rigid_body->setWorldTransform(btTransform(current_transform.getRotation(), bt_helpers::yete_to_bt_vector3(t_location)));
}

void rigid_body::set_world_rotation(const quaternion& t_rotation)
{
	const auto current_transform = m_bt_rigid_body->getWorldTransform();
	m_bt_rigid_body->setWorldTransform(btTransform(bt_helpers::yete_to_bt_quaternion(t_rotation), current_transform.getOrigin()));
}

void rigid_body::set_center_of_mass_offset(const vector3& t_com_offset)
{
	m_center_of_mass_offset = t_com_offset;
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

float rigid_body::get_simulating_physics() const
{
	return m_simulate_physics;
}

float rigid_body::get_mass() const
{
	return m_simulate_physics ? m_mass : 0.f;
}

float rigid_body::get_friction() const
{
	return m_friction;
}

float rigid_body::get_linear_damping() const
{
	return m_linear_damping;
}

float rigid_body::get_angular_damping() const
{
	return m_angular_damping;
}

float rigid_body::get_rolling_friction() const
{
	return m_rolling_friction;
}

float rigid_body::get_spinning_friction() const
{
	return m_spinning_friction;
}

float rigid_body::get_restitution() const
{
	return m_restitution;
}

float rigid_body::get_linear_sleeping_threshold() const
{
	return m_linear_sleeping_threshold;
}

float rigid_body::get_angular_sleeping_threshold() const
{
	return m_angular_sleeping_threshold;
}

vector3 rigid_body::get_rigid_body_location() const
{
	return bt_helpers::bt_to_yete_vector3(m_bt_rigid_body->getWorldTransform().getOrigin());
}

quaternion rigid_body::get_rigid_body_rotation() const
{
	return bt_helpers::bt_to_yete_quaternion(m_bt_rigid_body->getWorldTransform().getRotation());
}

transform rigid_body::get_rigid_body_transform() const
{
	return bt_helpers::bt_to_yete_transform(m_bt_rigid_body->getWorldTransform());
}

vector3 rigid_body::get_center_of_mass_offset() const
{
	return m_center_of_mass_offset;
}

vector3 rigid_body::get_linear_velocity() const
{
	return bt_helpers::bt_to_yete_vector3(m_bt_rigid_body->getLinearVelocity());
}

vector3 rigid_body::get_angular_velocity() const
{
	return bt_helpers::bt_to_yete_vector3(m_bt_rigid_body->getAngularVelocity());
}

vector3 rigid_body::get_velocity_at_point(const vector3& t_location) const
{
	const auto rel_location = world_to_relative_loc(t_location);
	return m_bt_rigid_body->getVelocityInLocalPoint(bt_helpers::yete_to_bt_vector3(rel_location));
}

vector3 rigid_body::world_to_relative_loc(const vector3& t_location) const
{
	// TODO: Is this correct? There is no way of testing right now, need to come back to this
	const auto location = get_rigid_body_location();
	const auto rotation = get_rigid_body_rotation();
	const auto rel_location = rotation.unrotate_vector(location - t_location);
	return rel_location;
}

void rigid_body::add_force(const vector3& t_force)
{
	m_bt_rigid_body->applyCentralForce(bt_helpers::yete_to_bt_vector3(t_force));
}

void rigid_body::add_force_at_location(const vector3& t_force, const vector3& t_location)
{
	const auto rel_location = world_to_relative_loc(t_location);
	m_bt_rigid_body->applyForce(bt_helpers::yete_to_bt_vector3(t_force), bt_helpers::yete_to_bt_vector3(rel_location));
}

void rigid_body::add_impulse(const vector3& t_impulse)
{
	m_bt_rigid_body->applyCentralImpulse(bt_helpers::yete_to_bt_vector3(t_impulse));
}

void rigid_body::add_impulse_at_location(const vector3& t_impulse, const vector3& t_location)
{
	const auto rel_location = world_to_relative_loc(t_location);
	m_bt_rigid_body->applyImpulse(bt_helpers::yete_to_bt_vector3(t_impulse), bt_helpers::yete_to_bt_vector3(t_location));
}

void rigid_body::add_torque(const vector3& t_torque)
{
	m_bt_rigid_body->applyTorque(bt_helpers::yete_to_bt_vector3(t_torque));
}

void rigid_body::add_torque_impulse(const vector3& t_torque_impulse)
{
	m_bt_rigid_body->applyTorqueImpulse(bt_helpers::yete_to_bt_vector3(t_torque_impulse));
}

void rigid_body::set_linear_velocity(const vector3& t_linear_velocity)
{
	m_bt_rigid_body->setLinearVelocity(bt_helpers::yete_to_bt_vector3(t_linear_velocity));
}

void rigid_body::set_angular_velocity(const vector3& t_angular_velocity)
{
	m_bt_rigid_body->setAngularVelocity(bt_helpers::yete_to_bt_vector3(t_angular_velocity));
}