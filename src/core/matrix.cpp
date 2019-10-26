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
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix
	const float n = t_z_near;
	const float f = t_z_far;

	const float scale = tan(t_fov * 0.5f * math::pi() / 180.f) * n;
	const float r = t_aspect * scale;
	const float l = -r;
	const float t = scale;
	const float b = -t;
	

	return {
		{ 2 * n / (r - l),		0,					0,						 0	 },
		{ 0,					2 * n / (t - b),	0,						 0	 },
		{ (r + l) / (r - l),	(t + b) / (t - b),	-(f + n) / (f - n),		-1.f },
		{ 0,					0,					-2 * f * n / (f - n),	 0	 },
	};
}

matrix4x4 matrix4x4::look_at(const vector3& t_eye, const vector3& t_target, const vector3& t_up)
{
	const vector3 forward = (t_target - t_eye).get_normalized();
	const vector3 right = vector3::cross(t_up, forward).get_normalized();
	const vector3 up = vector3::cross(forward, right);

	const matrix4x4 orientation = {
		{ forward.x, forward.y, forward.z,	0.f },
		{ right.x,	 right.y,	right.z,	0.f },
		{ t_up.x,	 t_up.y,	t_up.z,		0.f },
		{ 0.f,		 0.f,		0.f,		1.f },
	};

	const matrix4x4 translation = {
		{ 1.f,		0.f,		0.f,	0.f },
		{ 0.f,		1.f,		0.f,	0.f },
		{ 0.f,		0.f,		1.f,	0.f },
		{ -t_eye.x, -t_eye.y, -t_eye.z, 1.f },
	};

	/* TODO: Optimize with
	mat4 viewMatrix = {
		vec4(      xaxis.x,            yaxis.x,            zaxis.x,       0 ),
		vec4(      xaxis.y,            yaxis.y,            zaxis.y,       0 ),
		vec4(      xaxis.z,            yaxis.z,            zaxis.z,       0 ),
		vec4(-dot( xaxis, eye ), -dot( yaxis, eye ), -dot( zaxis, eye ),  1 )
	};
	*/

	return orientation * translation;
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