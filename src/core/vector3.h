#pragma once

class vector3
{
public:
	float x, y, z;

	vector3()
		: x(0.f)
		, y(0.f)
		, z(0.f)
	{}

	vector3(float t_x, float t_y, float t_z)
		: x(t_x)
		, y(t_y)
		, z(t_z)
	{}

	vector3(const vector3 &o)
		: x(o.x)
		, y(o.y)
		, z(o.z)
	{}

	inline bool operator==(const vector3& o) const
	{
		return x == o.x && y == o.y && z == o.z;
	}

	static const vector3 zero;
	static const vector3 up;
	static const vector3 right;
	static const vector3 forward;
};