#pragma once
#include "core.h"

class transform
{
public:
	vector3 location;
	quaternion rotation;
	float scale;

	transform()
		: location(vector3::zero)
		, rotation(quaternion::identity)
		, scale(1.f)
	{}

	transform(const vector3& t_location, const quaternion &rotation, float scale)
		: location(t_location)
		, rotation(rotation)
		, scale(scale)
	{}

	static const transform identity;

	inline matrix4x4 to_matrix() const
	{
		matrix4x4 out_matrix;

		out_matrix[3][0] = location.x;
		out_matrix[3][1] = location.y;
		out_matrix[3][2] = location.z;

		const float x2 = rotation.x + rotation.x;
		const float y2 = rotation.y + rotation.y;
		const float z2 = rotation.z + rotation.z;
		{
			const float xx2 = rotation.x * x2;
			const float yy2 = rotation.y * y2;
			const float zz2 = rotation.z * z2;

			out_matrix[0][0] = (1.0f - (yy2 + zz2)) * scale;
			out_matrix[1][1] = (1.0f - (xx2 + zz2)) * scale;
			out_matrix[2][2] = (1.0f - (xx2 + yy2)) * scale;
		}
		{
			const float yz2 = rotation.y * z2;
			const float wx2 = rotation.w * x2;

			out_matrix[2][1] = (yz2 - wx2) * scale;
			out_matrix[1][2] = (yz2 + wx2) * scale;
		}
		{
			const float xy2 = rotation.x * y2;
			const float wz2 = rotation.w * z2;

			out_matrix[1][0] = (xy2 - wz2) * scale;
			out_matrix[0][1] = (xy2 + wz2) * scale;
		}
		{
			const float xz2 = rotation.x * z2;
			const float wy2 = rotation.w * y2;

			out_matrix[2][0] = (xz2 + wy2) * scale;
			out_matrix[0][2] = (xz2 - wy2) * scale;
		}

		out_matrix[0][3] = 0.0f;
		out_matrix[1][3] = 0.0f;
		out_matrix[2][3] = 0.0f;
		out_matrix[3][3] = 1.0f;

		return out_matrix;
	}

};