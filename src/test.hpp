#pragma once
#include "core/core.h"
#include <cassert>

void test_matrix4x4_inverse()
{
	const matrix4x4 m = {
		{  1,  1,  1, -1 },
		{  1,  1, -1,  1 },
		{  1, -1,  1,  1 },
		{ -1,  1,  1,  1 },
	};

	const matrix4x4 result = {
		{  0.25f,  0.25f,  0.25f, -0.25f },
		{  0.25f,  0.25f, -0.25f,  0.25f },
		{  0.25f, -0.25f,  0.25f,  0.25f },
		{ -0.25f,  0.25f,  0.25f,  0.25f },
	};

	const float determinant = m.determinant();
	assert(determinant == -16.f);

	const matrix4x4 m_i = m.inverse();
	assert(m_i == result);

	log::info("Testing", "matrix4x4 inverse success");
}

void test_matrix4x4_multiplication()
{
/*
2 -5 -4 -1
3 5 4 2
5 -1 3 3
1 1 2 2

5 10 13 20
15 5 20 0
1 5 3 0
5 82 10 54

result:
-74 -107 -96 -14
104 239	171 168
28 306	84 262
32 189 59 128
*/
	const matrix4x4 m_1 = {
		{ 2, -5, -4, -1 },
		{ 3, 5, 4, 2 },
		{ 5, -1, 3, 3 },
		{ 1,  1, 2, 2 },
	};

	const matrix4x4 m_2 = {
		{ 5, 10, 13, 20 },
		{ 15, 5, 20, 0 },
		{ 1, 5, 3, 0 },
		{ 5, 82, 10, 54 },
	};

	const matrix4x4 result = {
		{ -74, -107, -96, -14 },
		{ 104, 239, 171, 168 },
		{ 28, 306, 84, 262 },
		{ 32, 189, 59, 128 },
	};

	const matrix4x4 m_prod = m_1 * m_2;
	assert(m_prod == result);

	log::info("Testing", "matrix4x4 multiplication success");
}

void test_matrix4x4_vector4_multiplication()
{
/*
2 -5 -4 -1
3 5 4 2
5 -1 3 3
1 1 2 2

1
2
3
4
*/
	const matrix4x4 m_1 = {
		{ 2, -5, -4, -1 },
		{ 3, 5, 4, 2 },
		{ 5, -1, 3, 3 },
		{ 1,  1, 2, 2 },
	};

	const vector4 v_1 = {
		1,
		2,
		3,
		4
	};

	const vector4 rh_result = {
		-24,
		33,
		24,
		17
	};

	const vector4 lh_result = {
		27,
		6,
		21,
		20
	};

	const vector4 rh_prod = m_1 * v_1;
	assert(rh_prod == rh_result);

	const vector4 lh_prod = v_1 * m_1;
	assert(lh_prod == lh_result);

	log::info("Testing", "matrix4x4 x vector4 success");
}

void test_make_rot_from()
{
	const matrix4x4 rot_result =
	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 },
	};

	const matrix4x4 x_rot = matrix4x4::make_rot_matrix_from_x(vector3::forward);
	const matrix4x4 y_rot = matrix4x4::make_rot_matrix_from_y(vector3::right);
	const matrix4x4 z_rot = matrix4x4::make_rot_matrix_from_z(vector3::up);

	assert(x_rot == rot_result);
	assert(y_rot == rot_result);
	assert(z_rot == rot_result);

	const matrix4x4 xy_rot = matrix4x4::make_rot_matrix_from_xy(vector3::forward, vector3::right);
	const matrix4x4 xz_rot = matrix4x4::make_rot_matrix_from_xz(vector3::forward, vector3::up);
	
	assert(xy_rot == rot_result);
	assert(xz_rot == rot_result);

	const matrix4x4 yx_rot = matrix4x4::make_rot_matrix_from_yx(vector3::right, vector3::forward);
	const matrix4x4 yz_rot = matrix4x4::make_rot_matrix_from_yz(vector3::right, vector3::up);

	assert(yx_rot == rot_result);
	assert(yz_rot == rot_result);

	const matrix4x4 zx_rot = matrix4x4::make_rot_matrix_from_zx(vector3::up, vector3::forward);
	const matrix4x4 zy_rot = matrix4x4::make_rot_matrix_from_zy(vector3::up, vector3::right);

	assert(zx_rot == rot_result);
	assert(zy_rot == rot_result);

	log::info("Testing", "matrix4x4 make_rot_from success");

}

void run_tests()
{
	test_matrix4x4_inverse();
	test_matrix4x4_multiplication();
	test_matrix4x4_vector4_multiplication();
	test_make_rot_from();
}