#include "matrix3x3.h"
#include "matrix4x4.h"
#include "math.h"
#include "log.h"

/* ********************************* */
/* *********** matrix3x3 *********** */
/* ********************************* */

matrix3x3 const matrix3x3::identity = matrix3x3(
	vector3(1, 0, 0),
	vector3(0, 1, 0),
	vector3(0, 0, 1)
);

matrix3x3 matrix3x3::transpose() const
{
	return
	{
		{ matrix[0][0], matrix[1][0], matrix[2][0] },
		{ matrix[0][1], matrix[1][1], matrix[2][1] },
		{ matrix[0][2], matrix[1][2], matrix[2][2] },
	};
}

float matrix3x3::determinant(float scalar) const
{
	const auto calc_2x2_deteminant = [](const float m2x2[2][2], float scalar)
	{
		return scalar * m2x2[0][0] * m2x2[1][1] - scalar * m2x2[0][1] * m2x2[1][0];
	};

	const float m_1[2][2] =
	{
		{ matrix[1][1], matrix[1][2] },
		{ matrix[2][1], matrix[2][2] },
	};

	const float m_2[2][2] =
	{
		{ matrix[1][0], matrix[1][2] },
		{ matrix[2][0], matrix[2][2] },
	};

	const float m_3[2][2] =
	{
		{ matrix[1][0], matrix[1][1] },
		{ matrix[2][0], matrix[2][1] },
	};

	return calc_2x2_deteminant(m_1, matrix[0][0] * scalar) 
		 - calc_2x2_deteminant(m_2, matrix[0][1] * scalar) 
		 + calc_2x2_deteminant(m_3, matrix[0][2] * scalar);
}

std::array<vector2[2], 9> matrix3x3::minor_matrices() const
{
	return std::array<vector2[2], 9>
	{
		{ 
			{{ matrix[1][1], matrix[1][2] }, 
			 { matrix[2][1], matrix[2][2] }},

			{{ matrix[1][0], matrix[1][2] }, 
			 { matrix[2][0], matrix[2][2] }},

			{{ matrix[1][0], matrix[1][1] }, 
			 { matrix[2][0], matrix[2][1] }},

			{{ matrix[0][1], matrix[0][2] }, 
			 { matrix[2][1], matrix[2][2] }},

			{{ matrix[0][0], matrix[0][2] }, 
			 { matrix[2][0], matrix[2][2] }},

			{{ matrix[0][0], matrix[0][1] }, 
			 { matrix[2][0], matrix[2][1] }},

			{{ matrix[0][1], matrix[0][2] },
			 { matrix[1][1], matrix[1][2] }},

			{{ matrix[0][0], matrix[0][2] },
			 { matrix[1][0], matrix[1][2] }},

			{{ matrix[0][0], matrix[0][1] },
			 { matrix[1][0], matrix[1][1] }},
		}
	};
}

matrix3x3 matrix3x3::inverse() const
{
	const matrix3x3 t_matrix = transpose();

	const std::array<vector2[2], 9> t_minor_matrices = t_matrix.minor_matrices();

	const auto calc_2x2_deteminant = [](const vector2 m2x2[2]) { return m2x2[0][0] * m2x2[1][1] - m2x2[0][1] * m2x2[1][0]; };

	const std::array<float, 9> t_minor_determinants =
	{
		calc_2x2_deteminant(t_minor_matrices[0]),
		calc_2x2_deteminant(t_minor_matrices[1]),
		calc_2x2_deteminant(t_minor_matrices[2]),
		calc_2x2_deteminant(t_minor_matrices[3]),
		calc_2x2_deteminant(t_minor_matrices[4]),
		calc_2x2_deteminant(t_minor_matrices[5]),
		calc_2x2_deteminant(t_minor_matrices[6]),
		calc_2x2_deteminant(t_minor_matrices[7]),
		calc_2x2_deteminant(t_minor_matrices[8]),
	};

	const matrix3x3 adj_matrix =
	{
		{  t_minor_determinants[0], -t_minor_determinants[1],  t_minor_determinants[2] },
		{ -t_minor_determinants[3],  t_minor_determinants[4], -t_minor_determinants[5] },
		{  t_minor_determinants[6], -t_minor_determinants[7],  t_minor_determinants[8] },
	};

	return (1.f / determinant()) * adj_matrix;
}

std::string matrix3x3::to_string() const
{
	return string_format::format_string("\n%f %f %f\n%f %f %f\n%f %f %f",
		matrix[0][0], matrix[0][1], matrix[0][2],
		matrix[1][0], matrix[1][1], matrix[1][2],
		matrix[2][0], matrix[2][1], matrix[2][2]
	);
}


/* ********************************* */
/* *********** matrix4x4 *********** */
/* ********************************* */

matrix4x4 const matrix4x4::identity = matrix4x4(
	vector4(1, 0, 0, 0),
	vector4(0, 1, 0, 0),
	vector4(0, 0, 1, 0),
	vector4(0, 0, 0, 1)
);

matrix4x4 matrix4x4::perspective(float t_fov, float t_aspect, float t_z_near, float t_z_far)
{
	const float ar = t_aspect;
	const float n = t_z_near;
	const float f = t_z_far;
	const float r = n - f;
	const float h_f = tanf(math::deg_to_rad(t_fov / 2.0f));

	return {
		{ 1.0f / (h_f * ar), 0,			 0,				0				 },
		{ 0,				 1.0f / h_f, 0,				0				 },
		{ 0,				 0,			 (-n - f) / r,  2.0f * f * n / r },
		{ 0,				 0,			 1.f,			0				 },
	};
}

matrix4x4 matrix4x4::look_at(const vector3& t_eye, const vector3& t_target, const vector3& t_up)
{
	const vector3 x_axis = (t_target - t_eye).get_normalized();				// +X Forward
	const vector3 y_axis = vector3::cross(t_up, x_axis).get_normalized();	// +Y Right
	const vector3 z_axis = vector3::cross(y_axis, x_axis);					// +Z Up

	const matrix4x4 view_matrix = {
		{ x_axis.x, y_axis.x, z_axis.x, -vector3::dot(x_axis, t_eye) },
		{ x_axis.y, y_axis.y, z_axis.y, -vector3::dot(y_axis, t_eye) },
		{ x_axis.z, y_axis.z, z_axis.z, -vector3::dot(z_axis, t_eye) },
		{ 0.f,		0.f,	  0.f,		1.f							 },
	};

	// TODO: converting from OpenGL to YETE coordinates here, should probably not be hard-coded like this
	/*const matrix4x4 view_matrix = {
		{ x_axis.x, y_axis.x, z_axis.x, -vector3::dot(y_axis, t_eye) },
		{ x_axis.y, y_axis.y, z_axis.y, -vector3::dot(-1 * z_axis, t_eye) },
		{ x_axis.z, y_axis.z, z_axis.z, -vector3::dot(x_axis, t_eye) },
		{ 0.f,		0.f,	  0.f,		1.f							 },
	};*/

	return view_matrix;
}

matrix4x4 matrix4x4::transpose() const
{
	return
	{
		{ matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0] },
		{ matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1] },
		{ matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2] },
		{ matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3] },
	};
}

float matrix4x4::determinant() const
{
	const matrix3x3 m_1 =
	{
		{ matrix[1][1], matrix[1][2], matrix[1][3] },
		{ matrix[2][1], matrix[2][2], matrix[2][3] },
		{ matrix[3][1], matrix[3][2], matrix[3][3] }
	};

	const matrix3x3 m_2 =
	{
		{ matrix[1][0], matrix[1][2], matrix[1][3] },
		{ matrix[2][0], matrix[2][2], matrix[2][3] },
		{ matrix[3][0], matrix[3][2], matrix[3][3] }
	};

	const matrix3x3 m_3 =
	{
		{ matrix[1][0], matrix[1][1], matrix[1][3] },
		{ matrix[2][0], matrix[2][1], matrix[2][3] },
		{ matrix[3][0], matrix[3][1], matrix[3][3] }
	};

	const matrix3x3 m_4 =
	{
		{ matrix[1][0], matrix[1][1], matrix[1][2] },
		{ matrix[2][0], matrix[2][1], matrix[2][2] },
		{ matrix[3][0], matrix[3][1], matrix[3][2] }
	};

	return m_1.determinant(matrix[0][0]) 
		 - m_2.determinant(matrix[0][1])
		 + m_3.determinant(matrix[0][2])
		 - m_4.determinant(matrix[0][3]);
}

std::array<matrix3x3, 16> matrix4x4::minor_matrices() const
{
	return std::array<matrix3x3, 16>
	{
		{ 
			{{ matrix[1][1], matrix[1][2], matrix[1][3] }, 
			 { matrix[2][1], matrix[2][2], matrix[2][3] },
			 { matrix[3][1], matrix[3][2], matrix[3][3] }},

			{{ matrix[1][0], matrix[1][2], matrix[1][3] }, 
			 { matrix[2][0], matrix[2][2], matrix[2][3] },
			 { matrix[3][0], matrix[3][2], matrix[3][3] }},

			{{ matrix[1][0], matrix[1][1], matrix[1][3] }, 
			 { matrix[2][0], matrix[2][1], matrix[2][3] },
			 { matrix[3][0], matrix[3][1], matrix[3][3] }},

			{{ matrix[1][0], matrix[1][1], matrix[1][2] }, 
			 { matrix[2][0], matrix[2][1], matrix[2][2] },
			 { matrix[3][0], matrix[3][1], matrix[3][2] }},

			{{ matrix[0][1], matrix[0][2], matrix[0][3] }, 
			 { matrix[2][1], matrix[2][2], matrix[2][3] },
			 { matrix[3][1], matrix[3][2], matrix[3][3] }},

			{{ matrix[0][0], matrix[0][2], matrix[0][3] }, 
			 { matrix[2][0], matrix[2][2], matrix[2][3] },
			 { matrix[3][0], matrix[3][2], matrix[3][3] }},

			{{ matrix[0][0], matrix[0][1], matrix[0][3] }, 
			 { matrix[2][0], matrix[2][1], matrix[2][3] },
			 { matrix[3][0], matrix[3][1], matrix[3][3] }},

			{{ matrix[0][0], matrix[0][1], matrix[0][2] }, 
			 { matrix[2][0], matrix[2][1], matrix[2][2] },
			 { matrix[3][0], matrix[3][1], matrix[3][2] }},

			{{ matrix[0][1], matrix[0][2], matrix[0][3] }, 
			 { matrix[1][1], matrix[1][2], matrix[1][3] },
			 { matrix[3][1], matrix[3][2], matrix[3][3] }},

			{{ matrix[0][0], matrix[0][2], matrix[0][3] }, 
			 { matrix[1][0], matrix[1][2], matrix[1][3] },
			 { matrix[3][0], matrix[3][2], matrix[3][3] }},

			{{ matrix[0][0], matrix[0][1], matrix[0][3] }, 
			 { matrix[1][0], matrix[1][1], matrix[1][3] },
			 { matrix[3][0], matrix[3][1], matrix[3][3] }},

			{{ matrix[0][0], matrix[0][1], matrix[0][2] }, 
			 { matrix[1][0], matrix[1][1], matrix[1][2] },
			 { matrix[3][0], matrix[3][1], matrix[3][2] }},

			{{ matrix[0][1], matrix[0][2], matrix[0][3] }, 
			 { matrix[1][1], matrix[1][2], matrix[1][3] },
			 { matrix[2][1], matrix[2][2], matrix[2][3] }},

			{{ matrix[0][0], matrix[0][2], matrix[0][3] }, 
			 { matrix[1][0], matrix[1][2], matrix[1][3] },
			 { matrix[2][0], matrix[2][2], matrix[2][3] }},

			{{ matrix[0][0], matrix[0][1], matrix[0][3] }, 
			 { matrix[1][0], matrix[1][1], matrix[1][3] },
			 { matrix[2][0], matrix[2][1], matrix[2][3] }},

			{{ matrix[0][0], matrix[0][1], matrix[0][2] }, 
			 { matrix[1][0], matrix[1][1], matrix[1][2] },
			 { matrix[2][0], matrix[2][1], matrix[2][2] }},
		}
	};
}

matrix4x4 matrix4x4::inverse() const
{
	const matrix4x4 t_matrix = transpose();
	const std::array<matrix3x3, 16> t_minor_matrices = t_matrix.minor_matrices();

	const std::array<float, 16> t_minor_determinants =
	{
		t_minor_matrices[0].determinant(),
		t_minor_matrices[1].determinant(),
		t_minor_matrices[2].determinant(),
		t_minor_matrices[3].determinant(),
		t_minor_matrices[4].determinant(),
		t_minor_matrices[5].determinant(),
		t_minor_matrices[6].determinant(),
		t_minor_matrices[7].determinant(),
		t_minor_matrices[8].determinant(),
		t_minor_matrices[9].determinant(),
		t_minor_matrices[10].determinant(),
		t_minor_matrices[11].determinant(),
		t_minor_matrices[12].determinant(),
		t_minor_matrices[13].determinant(),
		t_minor_matrices[14].determinant(),
		t_minor_matrices[15].determinant(),
	};

	const matrix4x4 adj_matrix =
	{
		{  t_minor_determinants[0], -t_minor_determinants[1],   t_minor_determinants[2],  -t_minor_determinants[3]  },
		{ -t_minor_determinants[4],  t_minor_determinants[5],  -t_minor_determinants[6],   t_minor_determinants[7]  },
		{  t_minor_determinants[8], -t_minor_determinants[9],   t_minor_determinants[10], -t_minor_determinants[11] },
		{ -t_minor_determinants[12], t_minor_determinants[13], -t_minor_determinants[14],  t_minor_determinants[15] },
	};

	return (1.f / determinant()) * adj_matrix;
}

std::string matrix4x4::to_string() const
{
	return string_format::format_string("\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
		matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
		matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
		matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
		matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]
	);
}