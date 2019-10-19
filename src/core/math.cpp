#include "core.h"

vector3 const vector3::zero		= vector3(0.f, 0.f, 0.f);
vector3 const vector3::up		= vector3(0.f, 0.f, 1.f);
vector3 const vector3::right	= vector3(0.f, 1.f, 0.f);
vector3 const vector3::forward	= vector3(1.f, 0.f, 0.f);

quaternion const quaternion::identity = quaternion(1.f, 0.f, 0.f, 0.f);

color const color::red		= color(255, 0, 0, 255);
color const color::green	= color(0, 255, 0, 255);
color const color::blue		= color(0, 0, 255, 255);