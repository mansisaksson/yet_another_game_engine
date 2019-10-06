#pragma once

class quaternion
{
public:
	float a, b, c, d;

	quaternion()
		: a(1.f)
		, b(0.f)
		, c(0.f)
		, d(0.f)
	{}

	quaternion(float a, float b, float c, float d)
		: a(a)
		, b(b)
		, c(c)
		, d(d)
	{}

	quaternion(const quaternion &o)
		: a(o.a)
		, b(o.b)
		, c(o.c)
		, d(o.d)
	{}

	static const quaternion identity;
};