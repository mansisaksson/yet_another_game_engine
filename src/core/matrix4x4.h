#pragma once
#include "vector4.h"
#include "matrix3x3.h"
#include <array>

/*
	row-major matrix 4x4
	acessed by [row][col]
*/
class matrix4x4
{
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


	/* Helper functions */

	static matrix4x4 perspective(float t_fov, float t_aspect, float t_z_near, float t_z_far);

	static matrix4x4 look_at(const vector3& t_eye, const vector3& t_target, const vector3& t_up);

	inline matrix4x4 transpose() const;

	inline float determinant() const;

	std::array<matrix3x3, 16> minor_matrices() const;

	matrix4x4 inverse() const;

	std::string to_string() const;

	
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
		/*
		TODO: Remove, kept for reference
		matrix4x4 tmp_matrix;
		for (int row = 0; row < 3; row++)
		{
			const vector4 row_values = { matrix[row][0], matrix[row][1], matrix[row][2], matrix[row][3] };
			for (int col = 0; col < 3; col++)
			{
				const vector4 col_values = { matrix[0][col], matrix[1][col], matrix[2][col], matrix[3][col] };
				tmp_matrix[row][col] = vector4::dot(row_values, col_values);
			}
		}
		*this = tmp_matrix;*/

		*this = 
		{
			{
				{ matrix[0][0] * rhs[0][0] + matrix[0][1] * rhs[1][0] + matrix[0][2] * rhs[2][0] + matrix[0][3] * rhs[3][0] },
				{ matrix[1][0] * rhs[0][0] + matrix[1][1] * rhs[1][0] + matrix[1][2] * rhs[2][0] + matrix[1][3] * rhs[3][0] },
				{ matrix[2][0] * rhs[0][0] + matrix[2][1] * rhs[1][0] + matrix[2][2] * rhs[2][0] + matrix[2][3] * rhs[3][0] },
				{ matrix[3][0] * rhs[0][0] + matrix[3][1] * rhs[1][0] + matrix[3][2] * rhs[2][0] + matrix[3][3] * rhs[3][0] },
			},
			{
				{ matrix[0][0] * rhs[0][1] + matrix[0][1] * rhs[1][1] + matrix[0][2] * rhs[2][1] + matrix[0][3] * rhs[3][1] },
				{ matrix[1][0] * rhs[0][1] + matrix[1][1] * rhs[1][1] + matrix[1][2] * rhs[2][1] + matrix[1][3] * rhs[3][1] },
				{ matrix[2][0] * rhs[0][1] + matrix[2][1] * rhs[1][1] + matrix[2][2] * rhs[2][1] + matrix[2][3] * rhs[3][1] },
				{ matrix[3][0] * rhs[0][1] + matrix[3][1] * rhs[1][1] + matrix[3][2] * rhs[2][1] + matrix[3][3] * rhs[3][1] },
			},
			{
				{ matrix[0][0] * rhs[0][2] + matrix[0][1] * rhs[1][2] + matrix[0][2] * rhs[2][2] + matrix[0][3] * rhs[3][2] },
				{ matrix[1][0] * rhs[0][2] + matrix[1][1] * rhs[1][2] + matrix[1][2] * rhs[2][2] + matrix[1][3] * rhs[3][2] },
				{ matrix[2][0] * rhs[0][2] + matrix[2][1] * rhs[1][2] + matrix[2][2] * rhs[2][2] + matrix[2][3] * rhs[3][2] },
				{ matrix[3][0] * rhs[0][2] + matrix[3][1] * rhs[1][2] + matrix[3][2] * rhs[2][2] + matrix[3][3] * rhs[3][2] }
			},
			{
				{ matrix[0][0] * rhs[0][3] + matrix[0][1] * rhs[1][3] + matrix[0][2] * rhs[2][3] + matrix[0][3] * rhs[3][3] },
				{ matrix[1][0] * rhs[0][3] + matrix[1][1] * rhs[1][3] + matrix[1][2] * rhs[2][3] + matrix[1][3] * rhs[3][3] },
				{ matrix[2][0] * rhs[0][3] + matrix[2][1] * rhs[1][3] + matrix[2][2] * rhs[2][3] + matrix[2][3] * rhs[3][3] },
				{ matrix[3][0] * rhs[0][3] + matrix[3][1] * rhs[1][3] + matrix[3][2] * rhs[2][3] + matrix[3][3] * rhs[3][3] }
			}
		};
		return *this;
	}

	inline matrix4x4& operator/=(const matrix4x4& rhs)
	{
		*this *= rhs.inverse();
		return *this;
	}

	template<typename U>
	inline matrix4x4 &operator*=(const U& scalar)
	{
		*this =
		{
			{ matrix[0][0] * scalar, matrix[0][1] * scalar, matrix[0][2] * scalar, matrix[0][3] * scalar },
			{ matrix[1][0] * scalar, matrix[1][1] * scalar, matrix[1][2] * scalar, matrix[1][3] * scalar },
			{ matrix[2][0] * scalar, matrix[2][1] * scalar, matrix[2][2] * scalar, matrix[2][3] * scalar },
			{ matrix[3][0] * scalar, matrix[3][1] * scalar, matrix[3][2] * scalar, matrix[3][3] * scalar },
		};
		return *this;
	}

	template<typename U>
	inline matrix4x4& operator/=(const U& scalar)
	{
		*this *= 1.f / scalar;
		return *this;
	}

	vector4& operator[](int idx)
	{
		assert(idx >= 0 && idx < 4 && "matrix4x4 - index out of range");
		return matrix[idx];
	}

	const vector4& operator[](int idx) const
	{
		assert(idx >= 0 && idx < 4 && "matrix4x4 - index out of range");
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
inline matrix4x4 operator*(const U& scalar, matrix4x4 rhs)
{
	rhs *= scalar;
	return rhs;
}

template<typename U>
inline matrix4x4 operator/(matrix4x4 lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}


/* vector4 operators */

inline vector4 operator*(const vector4 &lhs, const matrix4x4& rhs)
{
	return vector4(
		rhs.matrix[0][0] * lhs[0] + rhs.matrix[0][1] * lhs[1] + rhs.matrix[0][2] * lhs[2] + rhs.matrix[0][3] * lhs[3],
		rhs.matrix[1][0] * lhs[0] + rhs.matrix[1][1] * lhs[1] + rhs.matrix[1][2] * lhs[2] + rhs.matrix[1][3] * lhs[3],
		rhs.matrix[2][0] * lhs[0] + rhs.matrix[2][1] * lhs[1] + rhs.matrix[2][2] * lhs[2] + rhs.matrix[2][3] * lhs[3],
		rhs.matrix[3][0] * lhs[0] + rhs.matrix[3][1] * lhs[1] + rhs.matrix[3][2] * lhs[2] + rhs.matrix[3][3] * lhs[3]
	);
}

inline vector4 operator*(const matrix4x4& rhs, const vector4& lhs)
{
	return vector4(
		lhs[0] * rhs.matrix[0][0] + lhs[1] * rhs.matrix[1][0] + lhs[2] * rhs.matrix[2][0] + lhs[3] * rhs.matrix[3][0],
		lhs[0] * rhs.matrix[0][1] + lhs[1] * rhs.matrix[1][1] + lhs[2] * rhs.matrix[2][1] + lhs[3] * rhs.matrix[3][1],
		lhs[0] * rhs.matrix[0][2] + lhs[1] * rhs.matrix[1][2] + lhs[2] * rhs.matrix[2][2] + lhs[3] * rhs.matrix[3][2],
		lhs[0] * rhs.matrix[0][3] + lhs[1] * rhs.matrix[1][3] + lhs[2] * rhs.matrix[2][3] + lhs[3] * rhs.matrix[3][3]
	);
}