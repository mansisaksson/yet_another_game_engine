#pragma once

enum class shape_type
{
	invalid,
	box,
	sphere,
	capsule
};

class collision_shape
{
private:

	shape_type m_shape_type;

public:

	collision_shape()
		: m_shape_type(shape_type::invalid)
	{
	}

	collision_shape(shape_type shape_type)
		: m_shape_type(shape_type)
	{
	}

	inline shape_type get_shape_type() const
	{
		return m_shape_type;
	}

	inline bool is_box() const
	{
		return m_shape_type == shape_type::box;
	}

	inline bool is_sphere() const
	{
		return m_shape_type == shape_type::sphere;
	}

	inline bool is_capsule() const
	{
		return m_shape_type == shape_type::capsule;
	}
};