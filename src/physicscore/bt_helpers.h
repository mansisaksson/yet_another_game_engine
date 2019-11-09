#pragma once

#include "core/core.h"
#include <bullet/btBulletDynamicsCommon.h>

namespace bt_helpers
{
	/* Bullet to YETE */
	inline btQuaternion yete_to_bt_quaternion(const quaternion& quat)
	{
		return btQuaternion(quat.y, quat.z, -quat.x, quat.w);
	}

	inline btVector3 yete_to_bt_vector3(const vector3& vector)
	{
		return btVector3(vector.y, vector.z, -vector.x);
	}

	inline btTransform yete_to_bt_transform(const transform& transform)
	{
		return btTransform(yete_to_bt_quaternion(transform.rotation), yete_to_bt_vector3(transform.location));
	}

	/* YETE to Bullet */
	inline quaternion bt_to_yete_quaternion(const btQuaternion& bt_quat)
	{
		return quaternion(-bt_quat.getZ(), bt_quat.getX(), bt_quat.getY(), bt_quat.getW());
	}

	inline vector3 bt_to_yete_vector3(const btVector3& bt_vector)
	{
		return vector3(-bt_vector.getZ(), bt_vector.getX(), bt_vector.getY());
	}

	inline transform bt_to_yete_transform(const btTransform& bt_transform)
	{
		return transform(bt_to_yete_vector3(bt_transform.getOrigin()), bt_to_yete_quaternion(bt_transform.getRotation()), 1.f);
	}
}