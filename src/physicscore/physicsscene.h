#pragma once
#include "core/core.h"

class rigid_body;

class physics_scene
{
private:
	struct world_user_info
	{
		physics_scene* yete_phys_scene;
	};

	class btDefaultCollisionConfiguration* m_bt_collision_configuration;
	class btCollisionDispatcher* m_bt_dispatcher;
	struct btDbvtBroadphase* m_bt_broadphase;
	class btSequentialImpulseConstraintSolver* m_bt_solver;
	class btDiscreteDynamicsWorld* m_bt_dynamics_world;
	world_user_info* m_world_user_info;

public:
	physics_scene();
	virtual ~physics_scene();

	physics_scene(const vector3 &gravity);

	void simulate(float time);

	void add_rigid_body(const rigid_body &rigid_body);

	void remove_rigid_body(const rigid_body& rigid_body);

private:

	static void bt_phys_scene_tick(class btDynamicsWorld* world, float timeStep);

};