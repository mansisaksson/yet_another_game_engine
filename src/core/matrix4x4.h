#pragma once
#include "glm/glm.hpp"
#include "vector4.h"
#include <array>

class matrix4x4
{
private:
	matrix4x4(const glm::mat4& t_glm_mat4)
		: matrix({ t_glm_mat4[0], t_glm_mat4[1], t_glm_mat4[2], t_glm_mat4[3] })
	{}

	glm::mat4x4 to_glm() const
	{
		return glm::mat4x4(
			matrix[0].to_glm(),
			matrix[1].to_glm(),
			matrix[2].to_glm(),
			matrix[3].to_glm()
		);
	}

public:
	std::array<vector4, 4> matrix = { vector4::zero, vector4::zero, vector4::zero, vector4::zero };

	matrix4x4()
	{}

	matrix4x4(const float t_m[4][4])
		: matrix({ t_m[0], t_m[1], t_m[2], t_m[3] })
	{}

	matrix4x4(const vector4 o[4])
		: matrix({ o[0], o[1], o[2], o[3] })
	{}

	matrix4x4(const vector4 &v1, const vector4& v2, const vector4& v3, const vector4& v4)
		: matrix({ v1, v2, v3, v4 })
	{}

	/* statics */

	static const matrix4x4 identity;

	/* Operators */

	inline matrix4x4& operator=(const matrix4x4&rhs)
	{
		matrix[0] = rhs.matrix[0];
		matrix[1] = rhs.matrix[1];
		matrix[2] = rhs.matrix[2];
		matrix[3] = rhs.matrix[3];
		return *this;
	}

	inline matrix4x4& operator-=(const matrix4x4& rhs)
	{
		matrix[0] -= rhs.matrix[0];
		matrix[1] -= rhs.matrix[1];
		matrix[2] -= rhs.matrix[2];
		matrix[3] -= rhs.matrix[3];
		return *this;
	}

	inline matrix4x4& operator+=(const matrix4x4& rhs)
	{
		matrix[0] += rhs.matrix[0];
		matrix[1] += rhs.matrix[1];
		matrix[2] += rhs.matrix[2];
		matrix[3] += rhs.matrix[3];
		return *this;
	}

	inline matrix4x4& operator*=(const matrix4x4& rhs)
	{
		*this = matrix4x4(to_glm() * rhs.to_glm());
		return *this;
	}

	inline matrix4x4& operator/=(const matrix4x4& rhs)
	{
		*this = matrix4x4(to_glm() / rhs.to_glm());
		return *this;
	}

	template<typename U>
	inline matrix4x4 operator*=(const U& scalar)
	{
		return matrix4x4(matrix[0] * scalar, matrix[1] * scalar, matrix[2] * scalar, matrix[3] * scalar);
	}

	template<typename U>
	inline matrix4x4& operator/=(const U& scalar)
	{
		return matrix4x4(matrix[0] / scalar, matrix[1] / scalar, matrix[2] / scalar, matrix[3] / scalar);
	}

	vector4& operator[](int idx)
	{
		assert(idx > 0 && idx < 4 && "matrix4x4 - index out of range");
		return matrix[idx];
	}

	const vector4& operator[](int idx) const
	{
		assert(idx > 0 && idx < 4 && "matrix4x4 - index out of range");
		return matrix[idx];
	}
};


/* Comparison operators */

inline bool operator==(const matrix4x4& lhs, const matrix4x4& rhs) { return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2] && lhs[3] == rhs[3]; }
inline bool operator!=(const matrix4x4& lhs, const matrix4x4& rhs) { return !operator==(lhs, rhs); }


/* Assignment operators */

inline matrix4x4 operator+(matrix4x4 lhs, const matrix4x4& rhs)
{
	lhs += rhs;
	return lhs;
}

inline matrix4x4 operator-(matrix4x4 lhs, const matrix4x4& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline matrix4x4 operator*(matrix4x4 lhs, const matrix4x4& rhs)
{
	lhs *= rhs;
	return lhs;
}

inline matrix4x4 operator/(matrix4x4 lhs, const matrix4x4& rhs)
{
	lhs /= rhs;
	return lhs;
}

template<typename U>
inline matrix4x4 operator*(matrix4x4 lhs, const U& scalar)
{
	lhs *= scalar;
	return lhs;
}

template<typename U>
inline matrix4x4& operator/(matrix4x4 lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}