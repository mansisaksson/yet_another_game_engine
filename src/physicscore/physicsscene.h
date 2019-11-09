#pragma once
#include "core/core.h"

class rigid_body;

class physics_scene
{
private:
	class btDefaultCollisionConfiguration* m_bt_collision_configuration;
	class btCollisionDispatcher* m_bt_dispatcher;
	class btDbvtBroadphase* m_bt_broadphase;
	class btSequentialImpulseConstraintSolver* m_bt_solver;
	class btDiscreteDynamicsWorld* m_bt_dynamics_world;

public:
	physics_scene();
	virtual ~physics_scene();

	physics_scene(const vector3 &gravity);

	void simulate(float time);

	void add_rigid_body(const rigid_body &rigid_body);

	void remove_rigid_body(const rigid_body& rigid_body);

};