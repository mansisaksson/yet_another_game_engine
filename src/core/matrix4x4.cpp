#include "matrix4x4.h"
#include "core/core.h"

/* ********************************* */
/* *********** matrix4x4 *********** */
/* ********************************* */

matrix4x4 const matrix4x4::identity = matrix4x4(
	vector4(1, 0, 0, 0),
	vector4(0, 1, 0, 0),
	vector4(0, 0, 1, 0),
	vector4(0, 0, 0, 1)
);

matrix4x4 const matrix4x4::zero = matrix4x4(
	vector4(0, 0, 0, 0),
	vector4(0, 0, 0, 0),
	vector4(0, 0, 0, 0),
	vector4(0, 0, 0, 0)
);

matrix4x4 matrix4x4::perspective(float t_fov, float t_aspect_ratio, float t_z_near, float t_z_far)
{
	// http://in2gpu.com/2015/05/23/enter-the-matrix-and-projection/
	
	const float ar = t_aspect_ratio;
	const float h_f = tan(math::deg_to_rad(t_fov) / 2.f);

	const float n = t_z_near;
	const float f = t_z_far;

	return {
		{ 1.f / (ar * h_f),	0,			0,					0						},
		{ 0,				1.f / h_f,	0,					0						},
		{ 0,				0,			(-n - f) / (n - f),	(2.f * n * f) / (n - f) },
		{ 0,				0,			1.f,				0						},
	};

}

matrix4x4 matrix4x4::look_at(const vector3& t_eye, const vector3& t_target, const vector3& t_up)
{
	const vector3 x_axis = (t_target - t_eye).get_normalized();				// +X Forward
	const vector3 y_axis = vector3::cross(t_up, x_axis).get_normalized();	// +Y Right
	const vector3 z_axis = vector3::cross(x_axis, y_axis);					// +Z Up
	
	return {
		{ y_axis.x, y_axis.y, y_axis.z, -vector3::dot(y_axis, t_eye) },
		{ z_axis.x, z_axis.y, z_axis.z, -vector3::dot(z_axis, t_eye) },
		{ x_axis.x, x_axis.y, x_axis.z, -vector3::dot(x_axis, t_eye) },
		{ 0.f,		0.f,	  0.f,		1.f							 },
	};
}

matrix4x4 matrix4x4::make_rot_matrix_from_x(const vector3& vector)
{
	const vector3 forward = vector.get_normalized();
	const vector3 right = vector3::cross(vector3::up, forward).get_normalized();
	const vector3 up = vector3::cross(forward, right);

	return
	{
		{ forward.x,	forward.y,	forward.z,	0 },
		{ right.x,		right.y,	right.z,	0 },
		{ up.x,			up.y,		up.z,		0 },
		{ 0,			0,			0,			1 }
	};
}

matrix4x4 matrix4x4::make_rot_matrix_from_y(const vector3& vector)
{
	const vector3 right = vector.get_normalized();
	const vector3 forward = vector3::cross(right, vector3::up).get_normalized();
	const vector3 up = vector3::cross(forward, right);

	return
	{
		{ forward.x,	forward.y,	forward.z,	0 },
		{ right.x,		right.y,	right.z,	0 },
		{ up.x,			up.y,		up.z,		0 },
		{ 0,			0,			0,			1 }
	};
}

matrix4x4 matrix4x4::make_rot_matrix_from_z(const vector3& vector)
{
	const vector3 up = vector.get_normalized();
	const vector3 right = vector3::cross(up, vector3::forward).get_normalized();
	const vector3 forward = vector3::cross(right, up);

	return
	{
		{ forward.x,	forward.y,	forward.z,	0 },
		{ right.x,		right.y,	right.z,	0 },
		{ up.x,			up.y,		up.z,		0 },
		{ 0,			0,			0,			1 }
	};
}

matrix4x4 matrix4x4::make_rot_matrix_from_xy(const vector3& x_vector, const vector3& y_vector)
{
	const vector3 forward = x_vector.get_normalized();
	const vector3 up = vector3::cross(forward, y_vector);
	const vector3 right = vector3::cross(up, forward).get_normalized();

	return
	{
		{ forward.x,	forward.y,	forward.z,	0 },
		{ right.x,		right.y,	right.z,	0 },
		{ up.x,			up.y,		up.z,		0 },
		{ 0,			0,			0,			1 }
	};
}

matrix4x4 matrix4x4::make_rot_matrix_from_xz(const vector3& x_vector, const vector3& z_vector)
{
	const vector3 forward = x_vector.get_normalized();
	const vector3 right = vector3::cross(z_vector, forward).get_normalized();
	const vector3 up = vector3::cross(forward, right);

	return
	{
		{ forward.x,	forward.y,	forward.z,	0 },
		{ right.x,		right.y,	right.z,	0 },
		{ up.x,			up.y,		up.z,		0 },
		{ 0,			0,			0,			1 }
	};
}

matrix4x4 matrix4x4::make_rot_matrix_from_yx(const vector3& y_vector, const vector3& x_vector)
{
	const vector3 right = y_vector.get_normalized();
	const vector3 up = vector3::cross(x_vector, right).get_normalized();
	const vector3 forward = vector3::cross(right, up);

	return
	{
		{ forward.x,	forward.y,	forward.z,	0 },
		{ right.x,		right.y,	right.z,	0 },
		{ up.x,			up.y,		up.z,		0 },
		{ 0,			0,			0,			1 }
	};
}

matrix4x4 matrix4x4::make_rot_matrix_from_yz(const vector3& y_vector, const vector3& z_vector)
{
	const vector3 right = y_vector.get_normalized();
	const vector3 forward = vector3::cross(right, z_vector).get_normalized();
	const vector3 up = vector3::cross(forward, right);

	return
	{
		{ forward.x,	forward.y,	forward.z,	0 },
		{ right.x,		right.y,	right.z,	0 },
		{ up.x,			up.y,		up.z,		0 },
		{ 0,			0,			0,			1 }
	};
}

matrix4x4 matrix4x4::make_rot_matrix_from_zx(const vector3& z_vector, const vector3& x_vector)
{
	const vector3 up = z_vector.get_normalized();
	const vector3 right = vector3::cross(up, x_vector).get_normalized();
	const vector3 forward = vector3::cross(right, up);

	return
	{
		{ forward.x,	forward.y,	forward.z,	0 },
		{ right.x,		right.y,	right.z,	0 },
		{ up.x,			up.y,		up.z,		0 },
		{ 0,			0,			0,			1 }
	};
}

matrix4x4 matrix4x4::make_rot_matrix_from_zy(const vector3& z_vector, const vector3& y_vector)
{
	const vector3 up = z_vector.get_normalized();
	const vector3 forward = vector3::cross(y_vector, up).get_normalized();
	const vector3 right = vector3::cross(up, forward);

	return
	{
		{ forward.x,	forward.y,	forward.z,	0 },
		{ right.x,		right.y,	right.z,	0 },
		{ up.x,			up.y,		up.z,		0 },
		{ 0,			0,			0,			1 }
	};
}

quaternion matrix4x4::to_quaternion() const
{
	if (get_scaled_axis_x().nearly_zero() || get_scaled_axis_y().nearly_zero() || get_scaled_axis_z().nearly_zero())
		return quaternion::identity;

	const float trace = matrix[0][0] + matrix[1][1] + matrix[2][2];
	if (trace > 0)
	{
		const float s = 0.5f / sqrtf(trace + 1.0f);
		
		return {
			{ (matrix[1][2] - matrix[2][1]) * s },
			{ (matrix[2][0] - matrix[0][2]) * s },
			{ (matrix[0][1] - matrix[1][0]) * s },
			{ 0.25f / s }
		};
	}
	else 
	{
		const int i = [&]() 
		{
			int v;
			if (matrix[1][1] > matrix[0][0])
				v = 1;
			if (matrix[2][2] > matrix[i][i])
				v = 2;
			return v;
		}();

		const int nxt[3] = { 1, 2, 0 };
		const int j = nxt[i];
		const int k = nxt[j];

		const float inv_s = 1.f / sqrtf(matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.0f);
		const float s = 0.5f * inv_s;

		quaternion out_quat;
		out_quat[i] = 0.5f * (1.f / inv_s);
		out_quat[3] = (matrix[j][k] - matrix[k][j]) * s;
		out_quat[j] = (matrix[i][j] + matrix[j][i]) * s;
		out_quat[k] = (matrix[i][k] + matrix[k][i]) * s;

		return out_quat;
	}
}

float matrix4x4::determinant() const
{
	const auto s0 = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
	const auto s1 = matrix[0][0] * matrix[1][2] - matrix[1][0] * matrix[0][2];
	const auto s2 = matrix[0][0] * matrix[1][3] - matrix[1][0] * matrix[0][3];
	const auto s3 = matrix[0][1] * matrix[1][2] - matrix[1][1] * matrix[0][2];
	const auto s4 = matrix[0][1] * matrix[1][3] - matrix[1][1] * matrix[0][3];
	const auto s5 = matrix[0][2] * matrix[1][3] - matrix[1][2] * matrix[0][3];

	const auto c5 = matrix[2][2] * matrix[3][3] - matrix[3][2] * matrix[2][3];
	const auto c4 = matrix[2][1] * matrix[3][3] - matrix[3][1] * matrix[2][3];
	const auto c3 = matrix[2][1] * matrix[3][2] - matrix[3][1] * matrix[2][2];
	const auto c2 = matrix[2][0] * matrix[3][3] - matrix[3][0] * matrix[2][3];
	const auto c1 = matrix[2][0] * matrix[3][2] - matrix[3][0] * matrix[2][2];
	const auto c0 = matrix[2][0] * matrix[3][1] - matrix[3][0] * matrix[2][1];

	return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
}

std::array<std::array<vector3, 3>, 16> matrix4x4::minor_matrices() const
{
	return std::array<std::array<vector3, 3>, 16>
	{
		{ 
			{{{ matrix[1][1], matrix[1][2], matrix[1][3] }, 
			  { matrix[2][1], matrix[2][2], matrix[2][3] },
			  { matrix[3][1], matrix[3][2], matrix[3][3] }}},

			{{{ matrix[1][0], matrix[1][2], matrix[1][3] },
			  { matrix[2][0], matrix[2][2], matrix[2][3] },
			  { matrix[3][0], matrix[3][2], matrix[3][3] }}},

			{{{ matrix[1][0], matrix[1][1], matrix[1][3] },
			  { matrix[2][0], matrix[2][1], matrix[2][3] },
			  { matrix[3][0], matrix[3][1], matrix[3][3] }}},

			{{{ matrix[1][0], matrix[1][1], matrix[1][2] },
			  { matrix[2][0], matrix[2][1], matrix[2][2] },
			  { matrix[3][0], matrix[3][1], matrix[3][2] }}},

			{{{ matrix[0][1], matrix[0][2], matrix[0][3] },
			  { matrix[2][1], matrix[2][2], matrix[2][3] },
			  { matrix[3][1], matrix[3][2], matrix[3][3] }}},

			{{{ matrix[0][0], matrix[0][2], matrix[0][3] },
			  { matrix[2][0], matrix[2][2], matrix[2][3] },
			  { matrix[3][0], matrix[3][2], matrix[3][3] }}},

			{{{ matrix[0][0], matrix[0][1], matrix[0][3] },
			  { matrix[2][0], matrix[2][1], matrix[2][3] },
			  { matrix[3][0], matrix[3][1], matrix[3][3] }}},

			{{{ matrix[0][0], matrix[0][1], matrix[0][2] },
			  { matrix[2][0], matrix[2][1], matrix[2][2] },
			  { matrix[3][0], matrix[3][1], matrix[3][2] }}},

			{{{ matrix[0][1], matrix[0][2], matrix[0][3] },
			  { matrix[1][1], matrix[1][2], matrix[1][3] },
			  { matrix[3][1], matrix[3][2], matrix[3][3] }}},

			{{{ matrix[0][0], matrix[0][2], matrix[0][3] },
			  { matrix[1][0], matrix[1][2], matrix[1][3] },
			  { matrix[3][0], matrix[3][2], matrix[3][3] }}},

			{{{ matrix[0][0], matrix[0][1], matrix[0][3] },
			  { matrix[1][0], matrix[1][1], matrix[1][3] },
			  { matrix[3][0], matrix[3][1], matrix[3][3] }}},

			{{{ matrix[0][0], matrix[0][1], matrix[0][2] },
			  { matrix[1][0], matrix[1][1], matrix[1][2] },
			  { matrix[3][0], matrix[3][1], matrix[3][2] }}},

			{{{ matrix[0][1], matrix[0][2], matrix[0][3] },
			  { matrix[1][1], matrix[1][2], matrix[1][3] },
			  { matrix[2][1], matrix[2][2], matrix[2][3] }}},

			{{{ matrix[0][0], matrix[0][2], matrix[0][3] },
			  { matrix[1][0], matrix[1][2], matrix[1][3] },
			  { matrix[2][0], matrix[2][2], matrix[2][3] }}},

			{{{ matrix[0][0], matrix[0][1], matrix[0][3] },
			  { matrix[1][0], matrix[1][1], matrix[1][3] },
			  { matrix[2][0], matrix[2][1], matrix[2][3] }}},

			{{{ matrix[0][0], matrix[0][1], matrix[0][2] },
			  { matrix[1][0], matrix[1][1], matrix[1][2] },
			  { matrix[2][0], matrix[2][1], matrix[2][2] }}},
		}
	};
}

matrix4x4 matrix4x4::inverse() const
{
	const float s0 = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
	const float s1 = matrix[0][0] * matrix[1][2] - matrix[1][0] * matrix[0][2];
	const float s2 = matrix[0][0] * matrix[1][3] - matrix[1][0] * matrix[0][3];
	const float s3 = matrix[0][1] * matrix[1][2] - matrix[1][1] * matrix[0][2];
	const float s4 = matrix[0][1] * matrix[1][3] - matrix[1][1] * matrix[0][3];
	const float s5 = matrix[0][2] * matrix[1][3] - matrix[1][2] * matrix[0][3];
	
	const float c5 = matrix[2][2] * matrix[3][3] - matrix[3][2] * matrix[2][3];
	const float c4 = matrix[2][1] * matrix[3][3] - matrix[3][1] * matrix[2][3];
	const float c3 = matrix[2][1] * matrix[3][2] - matrix[3][1] * matrix[2][2];
	const float c2 = matrix[2][0] * matrix[3][3] - matrix[3][0] * matrix[2][3];
	const float c1 = matrix[2][0] * matrix[3][2] - matrix[3][0] * matrix[2][2];
	const float c0 = matrix[2][0] * matrix[3][1] - matrix[3][0] * matrix[2][1];

	// TODO: Should check for 0 determinant
	const float invdet = 1.f / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);

	auto inv = matrix4x4();

	inv[0][0] = (matrix[1][1] * c5 - matrix[1][2] * c4 + matrix[1][3] * c3) * invdet;
	inv[0][1] = (-matrix[0][1] * c5 + matrix[0][2] * c4 - matrix[0][3] * c3) * invdet;
	inv[0][2] = (matrix[3][1] * s5 - matrix[3][2] * s4 + matrix[3][3] * s3) * invdet;
	inv[0][3] = (-matrix[2][1] * s5 + matrix[2][2] * s4 - matrix[2][3] * s3) * invdet;

	inv[1][0] = (-matrix[1][0] * c5 + matrix[1][2] * c2 - matrix[1][3] * c1) * invdet;
	inv[1][1] = (matrix[0][0] * c5 - matrix[0][2] * c2 + matrix[0][3] * c1) * invdet;
	inv[1][2] = (-matrix[3][0] * s5 + matrix[3][2] * s2 - matrix[3][3] * s1) * invdet;
	inv[1][3] = (matrix[2][0] * s5 - matrix[2][2] * s2 + matrix[2][3] * s1) * invdet;

	inv[2][0] = (matrix[1][0] * c4 - matrix[1][1] * c2 + matrix[1][3] * c0) * invdet;
	inv[2][1] = (-matrix[0][0] * c4 + matrix[0][1] * c2 - matrix[0][3] * c0) * invdet;
	inv[2][2] = (matrix[3][0] * s4 - matrix[3][1] * s2 + matrix[3][3] * s0) * invdet;
	inv[2][3] = (-matrix[2][0] * s4 + matrix[2][1] * s2 - matrix[2][3] * s0) * invdet;

	inv[3][0] = (-matrix[1][0] * c3 + matrix[1][1] * c1 - matrix[1][2] * c0) * invdet;
	inv[3][1] = (matrix[0][0] * c3 - matrix[0][1] * c1 + matrix[0][2] * c0) * invdet;
	inv[3][2] = (-matrix[3][0] * s3 + matrix[3][1] * s1 - matrix[3][2] * s0) * invdet;
	inv[3][3] = (matrix[2][0] * s3 - matrix[2][1] * s1 + matrix[2][2] * s0) * invdet;

	return inv;
}
