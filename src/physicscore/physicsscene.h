#pragma once
#include "core/core.h"
#include "physicstypes.h"

class physics_scene
{
private:
	friend struct bt_yete_motion_state;

	class btDefaultCollisionConfiguration* m_bt_collision_configuration;
	class btCollisionDispatcher* m_bt_dispatcher;
	struct btDbvtBroadphase* m_bt_broadphase;
	class btSequentialImpulseConstraintSolver* m_bt_solver;
	class btDiscreteDynamicsWorld* m_bt_dynamics_world;

	std::vector<rigid_body*> m_dirty_bodies;

public:
	physics_scene();
	virtual ~physics_scene();

	physics_scene(const vector3 &gravity);

	void simulate(float time);

	void add_rigid_body(const rigid_body &rigid_body);

	void remove_rigid_body(const rigid_body& rigid_body);

private:

	void process_overlaps();

	static void bt_phys_scene_tick(class btDynamicsWorld* world, float timeStep);

};