#pragma once
#include "core/core.h"
#include <cassert>

void test_matrix3x3_inverse()
{
	const matrix3x3 m = {
		{ 1, 2, 3 },
		{ 0, 1, 4 },
		{ 5, 6, 0 },
	};

	const matrix3x3 result = {
		{ -24, 18, 5 },
		{ 20, -15, -4 },
		{ -5, 4, 1 },
	};

	const float determinant = m.determinant();
	assert(determinant == 1.f);

	const auto m_i = m.inverse();
	assert(m_i == result);

	log::info("Testing", "matrix3x3 inverse success");
}

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

void run_tests()
{
	test_matrix3x3_inverse();
	test_matrix4x4_inverse();
	test_matrix4x4_multiplication();
	test_matrix4x4_vector4_multiplication();
}