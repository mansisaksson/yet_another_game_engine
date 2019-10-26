#include "core.h"

vector2 const vector2::zero = vector2(0.f, 0.f);
vector2 const vector2::one = vector2(1.f, 1.f);

vector3 const vector3::zero		= vector3(0.f, 0.f, 0.f);
vector3 const vector3::one		= vector3(1.f, 1.f, 1.f);
vector3 const vector3::up		= vector3(0.f, 0.f, 1.f);
vector3 const vector3::right	= vector3(0.f, 1.f, 0.f);
vector3 const vector3::forward	= vector3(1.f, 0.f, 0.f);

vector4 const vector4::zero		= vector4(0.f, 0.f, 0.f, 0.f);

quaternion const quaternion::identity = quaternion(0.f, 0.f, 0.f, 1.f);

transform const transform::identity = transform(
	vector3(0, 0, 0),
	quaternion::identity,
	1.f
);

color const color::red		= color(255, 0, 0, 255);
color const color::green	= color(0, 255, 0, 255);
color const color::blue		= color(0, 0, 255, 255);