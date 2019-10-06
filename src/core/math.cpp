#include "core.h"

vector3 const vector3::zero		= vector3(0.f, 0.f, 0.f);
vector3 const vector3::up		= vector3(0.f, 0.f, 1.f);
vector3 const vector3::right	= vector3(0.f, 1.f, 0.f);
vector3 const vector3::forward	= vector3(1.f, 0.f, 0.f);

quaternion const quaternion::identity = quaternion(1.f, 0.f, 0.f, 0.f);