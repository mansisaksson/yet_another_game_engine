#pragma once
#include "core/core.h"
#include "assetmanager/assetptr.h"

class static_mesh;
class viewport;

class entity
{
public:
	transform transform;
	asset_ptr<static_mesh> static_mesh_ptr;

public:
	void draw(const viewport* viewport);

};
