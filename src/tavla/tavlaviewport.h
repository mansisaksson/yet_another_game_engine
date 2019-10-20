#pragma once

#include "tavla.h"
#include "assetmanager/assetptr.h"
#include "gameframework/scene.h"

class viewport;
class static_mesh;

class tavla_viewport : public tavla_base<tavla_viewport>
{
public:
	class slot : public tavla_base::slot<tavla_viewport::slot>
	{
	public:
		slot(const std::shared_ptr<tavla_viewport> & t_parent)
			: tavla_base::slot<tavla_viewport::slot>(t_parent)
		{}
	};

	std::shared_ptr<tavla_viewport::slot> add_slot()
	{
		return add_slot_of_type<tavla_viewport::slot>();
	}

private:

	std::shared_ptr<viewport> m_viewport;

public:
	float fov;
	float near_plane;
	float far_plane;

public:
	tavla_viewport();

	// ~begin tavla interface
	virtual void construct() override;
	virtual void destruct() override;
	virtual void tick(float t_delta_time) override;
	virtual void draw() override;
	// ~end tavla interface

	// TODO: TEMP, Remove!
	asset_ptr<static_mesh> static_mesh_ptr;
};