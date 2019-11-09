#include "physicsscene.h"
#include "rigidbody.h"
#include "bt_helpers.h"

physics_scene::physics_scene()
	: physics_scene(vector3(0, 0, -10))
{

}

physics_scene::~physics_scene()
{
	delete m_bt_collision_configuration;
	delete m_bt_dispatcher;
	delete m_bt_broadphase;
	delete m_bt_solver;
	delete m_bt_dynamics_world;
}

physics_scene::physics_scene(const vector3& gravity)
{
	// collision configuration contains default setup for memory, collision setup
	m_bt_collision_configuration = new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	// use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_bt_dispatcher = new btCollisionDispatcher(m_bt_collision_configuration);
	
	m_bt_broadphase = new btDbvtBroadphase();

	// the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_bt_solver = new btSequentialImpulseConstraintSolver;
	
	m_bt_dynamics_world = new btDiscreteDynamicsWorld(m_bt_dispatcher, m_bt_broadphase, m_bt_solver, m_bt_collision_configuration);
	m_bt_dynamics_world->setGravity(bt_helpers::yete_to_bt_vector3(gravity));
}

void physics_scene::simulate(float time)
{
	m_bt_dynamics_world->stepSimulation(time, 1, 1.f / 60.f);
}

void physics_scene::add_rigid_body(const rigid_body& rigid_body)
{
	m_bt_dynamics_world->addRigidBody(rigid_body.m_bt_rigid_body);
}

void physics_scene::remove_rigid_body(const rigid_body& rigid_body)
{
	m_bt_dynamics_world->removeRigidBody(rigid_body.m_bt_rigid_body);
}