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
	m_bt_dynamics_world->setInternalTickCallback(physics_scene::bt_phys_scene_tick, this, false);
}

void physics_scene::simulate(float time)
{
	m_bt_dynamics_world->stepSimulation(time, 1, 1.f / 60.f);
	process_overlaps();
}

void physics_scene::add_rigid_body(const rigid_body& rigid_body)
{
	m_bt_dynamics_world->addRigidBody(rigid_body.m_bt_rigid_body, rigid_body.m_blocking_group, rigid_body.m_blocking_mask);
}

void physics_scene::remove_rigid_body(const rigid_body& rigid_body)
{
	m_bt_dynamics_world->removeRigidBody(rigid_body.m_bt_rigid_body);
}

void physics_scene::process_overlaps()
{
	struct yete_contact_result_callback : public btCollisionWorld::ContactResultCallback
	{
		std::set<rigid_body*> m_overlapping_bodies;

		yete_contact_result_callback(uint32_t t_overlap_group, uint32_t t_overlap_mask)
		{
			m_collisionFilterGroup = t_overlap_group;
			m_collisionFilterMask = t_overlap_mask;
			m_closestDistanceThreshold = 0;
		}

		bool needsCollision(btBroadphaseProxy* proxy0) const override
		{
			btCollisionObject* collision_obj = static_cast<btCollisionObject*>(proxy0->m_clientObject);
			const auto other_overlap_group = static_cast<rigid_body*>(collision_obj->getUserPointer())->m_overlap_group;
			const auto other_overlap_mask = static_cast<rigid_body*>(collision_obj->getUserPointer())->m_overlap_mask;
			return ((other_overlap_group & m_collisionFilterMask) != 0) && ((m_collisionFilterGroup & other_overlap_mask) != 0);
		}

		btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
		{
			rigid_body* other_body = static_cast<rigid_body*>(colObj1Wrap->getCollisionObject()->getUserPointer());
			if (std::find(m_overlapping_bodies.begin(), m_overlapping_bodies.end(), other_body) == m_overlapping_bodies.end())
				m_overlapping_bodies.insert(other_body);
			return 0;
		}
	};

	// TODO: Looping over all bodies in the scene, ideally we would only loop over those that has moved
	for (int i = 0; i< m_bt_dynamics_world->getCollisionObjectArray().size(); i++)
	{
		btCollisionObject* collision_object = m_bt_dynamics_world->getCollisionObjectArray()[i];
		rigid_body* body = static_cast<rigid_body*>(collision_object->getUserPointer());

		yete_contact_result_callback bt_contact_result_callback(body->m_overlap_group, body->m_overlap_mask);
		m_bt_dynamics_world->contactTest(body->m_bt_rigid_body, bt_contact_result_callback);

		const auto &old_overlapping_bodies = body->m_overlapping_bodies;
		const auto &new_overlapping_bodies = bt_contact_result_callback.m_overlapping_bodies;
		
		std::set<rigid_body*> begin_overlaps;
		std::set_difference(
			new_overlapping_bodies.begin(), new_overlapping_bodies.end(),
			old_overlapping_bodies.begin(), old_overlapping_bodies.end(),
			std::inserter(begin_overlaps, begin_overlaps.begin())
		);

		std::set<rigid_body*> end_overlaps;
		std::set_difference(
			old_overlapping_bodies.begin(), old_overlapping_bodies.end(),
			new_overlapping_bodies.begin(), new_overlapping_bodies.end(),
			std::inserter(end_overlaps, end_overlaps.begin())
		);
		
		body->m_overlapping_bodies = new_overlapping_bodies;

		for (rigid_body* new_overlapping_body : begin_overlaps)
			body->on_begin_overlap.broadcast(*new_overlapping_body);

		for (rigid_body* end_overlapping_body : end_overlaps)
			body->on_end_overlap.broadcast(*end_overlapping_body);
	}
}

void physics_scene::bt_phys_scene_tick(class btDynamicsWorld* world, float timeStep)
{
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
					rigid_body_a,
					rigid_body_b,
					bt_helpers::bt_to_yete_vector3(pt.getPositionWorldOnA()),
					bt_helpers::bt_to_yete_vector3(pt.m_normalWorldOnB)
				});

				hit_results_b.push_back(hit_result
				{
					rigid_body_b,
					rigid_body_a,
					bt_helpers::bt_to_yete_vector3(pt.getPositionWorldOnB()),
					bt_helpers::bt_to_yete_vector3(pt.m_normalWorldOnB)
				});
			}
		}

		rigid_body_a->on_hit.broadcast(hit_results_a);
		rigid_body_b->on_hit.broadcast(hit_results_b);
	}
}