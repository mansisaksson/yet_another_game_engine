#pragma once
#include "glm/glm.hpp"

class vector2
{
private:
	vector2(const glm::vec2 &t_glm_vec)
		: x(t_glm_vec.x)
		, y(t_glm_vec.y)
	{}

	glm::vec2 to_glm() const
	{
		return glm::vec2(x, y);
	}

public:
	float x, y;

	vector2()
		: x(0)
		, y(0)
	{}

	vector2(float t_x, float t_y)
		: x(t_x)
		, y(t_y)
	{}

	vector2(const vector2& o)
		: x(o.x)
		, y(o.y)
	{}


	/* statics */

	static const vector2 zero;


	/* vector math */

	void normalize()
	{
		*this = vector2(glm::normalize(to_glm()));
	}

	vector2 get_normalized() const
	{
		return vector2(glm::normalize(to_glm()));
	}


	/* Operators */

	inline vector2& operator=(const vector2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	inline vector2& operator-=(const vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	inline vector2& operator+=(const vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	inline vector2& operator*=(const vector2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	inline vector2& operator/=(const vector2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	template<typename U>
	inline vector2 operator*=(const U& scalar)
	{
		return vector2(x * scalar, y * scalar, z * scalar);
	}

	template<typename U>
	inline vector2& operator/=(const U& scalar)
	{
		return vector2(x / scalar, y / scalar, z / scalar);
	}

	float& operator[](int idx)
	{
		assert(idx > 0 && idx < 2 && "vector2 - index out of range");

		switch (idx)
		{
		case 0: return x;
		case 1: return y;
		}
	}

	const float& operator[](int idx) const
	{
		assert(idx > 0 && idx < 2 && "vector2 - index out of range");

		switch (idx)
		{
		case 0: return x;
		case 1: return y;
		}
	}

};


/* Comparison operators */

inline bool operator==(const vector2& lhs, const vector2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
inline bool operator!=(const vector2& lhs, const vector2& rhs) { return !operator==(lhs, rhs); }


/* Assignment operators */

inline vector2 operator+(vector2 lhs, const vector2& rhs)
{
	lhs += rhs;
	return lhs;
}

inline vector2 operator-(vector2 lhs, const vector2& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline vector2 operator*(vector2 lhs, const vector2& rhs)
{
	lhs *= rhs;
	return lhs;
}

inline vector2 operator/(vector2 lhs, const vector2& rhs)
{
	lhs /= rhs;
	return lhs;
}

template<typename U>
inline vector2 operator*(vector2 lhs, const U& scalar)
{
	lhs *= scalar;
	return lhs;
}

template<typename U>
inline vector2& operator/(vector2 lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}