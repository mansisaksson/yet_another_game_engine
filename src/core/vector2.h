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

	static const vector2 zero;

	void normalize()
	{
		*this = glm::normalize(to_glm());
	}

	vector2 get_normalized() const
	{
		return glm::normalize(to_glm());
	}


	/* ~Begin: Operators */

	inline bool operator==(const vector2& o) const
	{
		return x == o.x && y == o.y;
	}

	inline vector2& operator=(const vector2& o)
	{
		x = o.x;
		y = o.y;
		return *this;
	}

	inline vector2& operator-(const vector2& o)
	{
		x -= o.x;
		y -= o.y;
		return *this;
	}

	inline vector2& operator+(const vector2& o)
	{
		x += o.x;
		y += o.y;
		return *this;
	}

	inline vector2& operator-=(const vector2& o)
	{
		*this = *this - o;
		return *this;
	}

	inline vector2& operator+=(const vector2& o)
	{
		*this = *this + o;
		return *this;
	}

	template<typename U>
	inline vector2& operator*(const U& scalar)
	{
		return vector2(x * scalar, y * scalar);
	}

	template<typename U>
	inline vector2& operator/(const U& scalar)
	{
		return vector2(x / scalar, y / scalar);
	}

	/* ~End: Operators */


};