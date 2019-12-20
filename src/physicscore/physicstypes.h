#pragma once
#include "core/core.h"

class collision_shape;
class box_shape;
class sphere_shape;
class capsule_shape;
class rigid_body;

struct hit_result
{
	rigid_body* body;
	rigid_body* other_body;
	vector3 hit_location;
	vector3 hit_normal;
};

enum class collision_type
{
	world_static,
	kinematic,
	simulated
};

enum class collision_channel : uint32_t
{
	channel_1		= 0x1,
	channel_2		= 0x2,
	channel_3		= 0x4,
	channel_4		= 0x8,
	channel_5		= 0x10,
	channel_6		= 0x20,
	channel_7		= 0x40,
	channel_8		= 0x80,
	channel_9		= 0x100,
	channel_10		= 0x200,
	channel_11		= 0x400,
	channel_12		= 0x800,
	channel_13		= 0x1000,
	channel_14		= 0x2000,
	channel_15		= 0x4000,
	channel_16		= 0x8000,
	channel_17		= 0x10000,
	channel_18		= 0x20000,
	channel_19		= 0x40000,
	channel_20		= 0x80000,
	channel_21		= 0x100000,
	channel_22		= 0x200000,
	channel_23		= 0x400000,
	channel_24		= 0x800000,
	channel_25		= 0x1000000,
	channel_26		= 0x2000000,
	channel_27		= 0x4000000,
	channel_28		= 0x8000000,
	channel_29		= 0x10000000,
	channel_30		= 0x20000000,
	channel_31		= 0x40000000,
	channel_32		= 0x80000000,
};

enum class collision_response
{
	ignore,
	block,
	overlap,
	block_and_overlap,
};