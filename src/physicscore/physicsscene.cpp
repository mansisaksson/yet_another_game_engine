#include "physicsscene.h"
#include "rigidbody.h"
#include "bt_helpers.h"
#include "bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h"

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
	delete m_world_user_info;
}

physics_scene::physics_scene(const vector3& gravity)
{
	// collision configuration contains default setup for memory, collision setup
	m_bt_collision_configuration = new btDefaultCollisionConfiguration();

	// use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_bt_dispatcher = new btCollisionDispatcher(m_bt_collision_configuration);
	
	m_bt_broadphase = new btDbvtBroadphase();

	// the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_bt_solver = new btSequentialImpulseConstraintSolver;
	
	m_bt_dynamics_world = new btDiscreteDynamicsWorld(m_bt_dispatcher, m_bt_broadphase, m_bt_solver, m_bt_collision_configuration);
	m_bt_dynamics_world->setGravity(bt_helpers::yete_to_bt_vector3(gravity));

	m_world_user_info = new world_user_info{ this };
	m_bt_dynamics_world->setInternalTickCallback(physics_scene::bt_phys_scene_tick, m_world_user_info, false);
}

void physics_scene::simulate(float time)
{
	m_bt_dynamics_world->stepSimulation(time, 1, 1.f / 60.f);

	// TODO: This: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=11635
	//for (rigid_body* body1 : dirty_bodies)
	//{
	//	m_bt_dynamics_world->contactTest();
	//	for (rigid_body* body2 : dirty_bodies)
	//	{
	//		if (body1 != body2 && body1->m_overlap_group & body2->m_overlap_mask)
	//		{
	//			btCollisionWorld::ContactResultCallback bt_contact_result_callback;
	//			//m_bt_dynamics_world->contactPairTest(body1->m_bt_rigid_body, body2->m_bt_rigid_body, )
	//			//body1->m_bt_rigid_body
	//		}
	//	}
	//}
}

void physics_scene::add_rigid_body(const rigid_body& rigid_body)
{
	m_bt_dynamics_world->addRigidBody(rigid_body.m_bt_rigid_body, rigid_body.m_blocking_group, rigid_body.m_blocking_mask);
}

void physics_scene::remove_rigid_body(const rigid_body& rigid_body)
{
	m_bt_dynamics_world->removeRigidBody(rigid_body.m_bt_rigid_body);
}

void physics_scene::bt_phys_scene_tick(class btDynamicsWorld* world, float timeStep)
{
	const world_user_info* user_info = static_cast<const world_user_info*>(world->getWorldUserInfo());

	const int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		const btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* ob_a = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* ob_b = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		const btRigidBody* bt_rigid_body_a = btRigidBody::upcast(ob_a);
		const btRigidBody* bt_rigid_body_b = btRigidBody::upcast(ob_b);

		rigid_body* rigid_body_a = rigid_body::bt_to_yete_rigid_body(bt_rigid_body_a);
		rigid_body* rigid_body_b = rigid_body::bt_to_yete_rigid_body(bt_rigid_body_b);

		const int numContacts = contactManifold->getNumContacts();
		std::vector<hit_result> hit_results_a;
		std::vector<hit_result> hit_results_b;
		hit_results_a.reserve(numContacts);
		hit_results_b.reserve(numContacts);

		for (int j = 0; j < numContacts; j++)
		{
			const btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if (pt.getDistance() < 0.f)
			{
				hit_results_a.push_back(hit_result
				{
					rigid_body_b,
					bt_helpers::bt_to_yete_vector3(pt.getPositionWorldOnA()),
					bt_helpers::bt_to_yete_vector3(pt.m_normalWorldOnB)
				});

				hit_results_b.push_back(hit_result
				{
					rigid_body_a,
					bt_helpers::bt_to_yete_vector3(pt.getPositionWorldOnB()),
					bt_helpers::bt_to_yete_vector3(pt.m_normalWorldOnB)
				});
			}
		}

		//rigid_body_a->on_hit.broadcast(hit_results_a);
		//rigid_body_b->on_hit.broadcast(hit_results_b);
	}
}