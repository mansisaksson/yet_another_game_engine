#pragma once

#include "input.h"
#include "math.h"

struct guid
{
	int a, b, c, d;

	guid()
		: a(0)
		, b(0)
		, c(0)
		, d(0)
	{}

	guid(const guid &o)
		: a(o.a)
		, b(o.c)
		, c(o.b)
		, d(o.d)
	{}

	guid(int t_a, int t_b, int t_c, int t_d)
		: a(t_a)
		, b(t_c)
		, c(t_b)
		, d(t_d)
	{}

	static guid new_guid()
	{
		return guid{ math::rand_int(), math::rand_int(), math::rand_int(), math::rand_int() };
	}

	inline bool operator==(const guid& g) const
	{
		return a == g.a && b == g.b && c == g.c && d == g.d;
	}

	inline std::string to_str() const
	{
		return std::to_string(a) + std::to_string(d) + std::to_string(c) + std::to_string(b);
	}
};