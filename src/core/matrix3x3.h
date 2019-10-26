#pragma once
#include "vector3.h"
#include "vector2.h"
#include <array>

/*
	row-major matrix 3x3
	acessed by [row][col]
*/
class matrix3x3
{
public:
	std::array<vector3, 3> matrix = { vector3::zero, vector3::zero, vector3::zero };

	matrix3x3()
	{}

	matrix3x3(const float t_m[3][3])
		: matrix({ t_m[0], t_m[1], t_m[2] })
	{}

	matrix3x3(const vector3 o[4])
		: matrix({ o[0], o[1], o[2] })
	{}

	matrix3x3(const vector3& v1, const vector3& v2, const vector3& v3)
		: matrix({ v1, v2, v3 })
	{}

	
	/* statics */

	static const matrix3x3 identity;


	/* Helper functions */

	inline matrix3x3 transpose() const;

	inline float determinant(float scalar = 1.f) const;

	std::array<vector2[2], 9> minor_matrices() const;

	matrix3x3 inverse() const;

	std::string to_string() const;


	/* Operators */


	inline matrix3x3& operator=(const matrix3x3& rhs)
	{
		matrix[0] = rhs.matrix[0];
		matrix[1] = rhs.matrix[1];
		matrix[2] = rhs.matrix[2];
		return *this;
	}

	inline matrix3x3& operator-=(const matrix3x3& rhs)
	{
		matrix[0] -= rhs.matrix[0];
		matrix[1] -= rhs.matrix[1];
		matrix[2] -= rhs.matrix[2];
		return *this;
	}

	inline matrix3x3& operator+=(const matrix3x3& rhs)
	{
		matrix[0] += rhs.matrix[0];
		matrix[1] += rhs.matrix[1];
		matrix[2] += rhs.matrix[2];
		return *this;
	}

	inline matrix3x3& operator*=(const matrix3x3& rhs)
	{
		*this =
		{
			{
				{ matrix[0][0] * rhs[0][0] + matrix[0][1] * rhs[1][0] + matrix[0][2] * rhs[2][0] },
				{ matrix[1][0] * rhs[0][0] + matrix[1][1] * rhs[1][0] + matrix[1][2] * rhs[2][0] },
				{ matrix[2][0] * rhs[0][0] + matrix[2][1] * rhs[1][0] + matrix[2][2] * rhs[2][0] },
			},
			{
				{ matrix[0][0] * rhs[0][1] + matrix[0][1] * rhs[1][1] + matrix[0][2] * rhs[2][1] },
				{ matrix[1][0] * rhs[0][1] + matrix[1][1] * rhs[1][1] + matrix[1][2] * rhs[2][1] },
				{ matrix[2][0] * rhs[0][1] + matrix[2][1] * rhs[1][1] + matrix[2][2] * rhs[2][1] },
			},
			{
				{ matrix[0][0] * rhs[0][2] + matrix[0][1] * rhs[1][2] + matrix[0][2] * rhs[2][2] },
				{ matrix[1][0] * rhs[0][2] + matrix[1][1] * rhs[1][2] + matrix[1][2] * rhs[2][2] },
				{ matrix[2][0] * rhs[0][2] + matrix[2][1] * rhs[1][2] + matrix[2][2] * rhs[2][2] },
			}
		};
		return *this;
	}

	inline matrix3x3& operator/=(const matrix3x3& rhs)
	{
		*this *= rhs.inverse();
		return *this;
	}

	template<typename U>
	inline matrix3x3& operator*=(const U& scalar)
	{
		*this =
		{ 
			{ matrix[0][0] * scalar, matrix[0][1] * scalar, matrix[0][2] * scalar },
			{ matrix[1][0] * scalar, matrix[1][1] * scalar, matrix[1][2] * scalar },
			{ matrix[2][0] * scalar, matrix[2][1] * scalar, matrix[2][2] * scalar },
		};
		return *this;
	}

	template<typename U>
	inline matrix3x3& operator/=(const U& scalar)
	{
		*this =
		{
			{ matrix[0][0] / scalar, matrix[0][1] / scalar, matrix[0][2] / scalar },
			{ matrix[1][0] / scalar, matrix[1][1] / scalar, matrix[1][2] / scalar },
			{ matrix[2][0] / scalar, matrix[2][1] / scalar, matrix[2][2] / scalar },
		};
		return *this;
	}

	vector3& operator[](int idx)
	{
		assert(idx >= 0 && idx < 4 && "matrix3x3 - index out of range");
		return matrix[idx];
	}

	const vector3& operator[](int idx) const
	{
		assert(idx >= 0 && idx < 4 && "matrix3x3 - index out of range");
		return matrix[idx];
	}
};


/* Comparison operators */

inline bool operator==(const matrix3x3& lhs, const matrix3x3& rhs) { return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2]; }
inline bool operator!=(const matrix3x3& lhs, const matrix3x3& rhs) { return !operator==(lhs, rhs); }


/* Assignment operators */

inline matrix3x3 operator+(matrix3x3 lhs, const matrix3x3& rhs)
{
	lhs += rhs;
	return lhs;
}

inline matrix3x3 operator-(matrix3x3 lhs, const matrix3x3& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline matrix3x3 operator*(matrix3x3 lhs, const matrix3x3& rhs)
{
	lhs *= rhs;
	return lhs;
}

inline matrix3x3 operator/(matrix3x3 lhs, const matrix3x3& rhs)
{
	lhs /= rhs;
	return lhs;
}

template<typename U>
inline matrix3x3 operator*(matrix3x3 lhs, const U& scalar)
{
	lhs *= scalar;
	return lhs;
}

template<typename U>
inline matrix3x3 operator*(const U& scalar, matrix3x3 rhs)
{
	rhs *= scalar;
	return rhs;
}

template<typename U>
inline matrix3x3 operator/(matrix3x3 lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}
