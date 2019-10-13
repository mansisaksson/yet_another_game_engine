#pragma once
#include "vertex.h"

struct indexed_model
{
	std::vector<vector3> positions;
	std::vector<vector2> tex_coords;
	std::vector<vector3> normals;
	std::vector<uint32_t> indices;
};

class static_mesh
{
private:
	static_mesh() = delete;
	static_mesh(const static_mesh&) = delete;
	static_mesh(static_mesh&&) = delete;
	bool operator=(const static_mesh&) = delete;

protected:

	static_mesh(const indexed_model& model);
	virtual ~static_mesh();

public:
	static std::shared_ptr<static_mesh> create_mesh(const indexed_model& model);
	static std::shared_ptr<static_mesh> load_mesh(const std::string &file_path);

	virtual void draw() = 0;

protected:

	virtual void init_mesh(const indexed_model& model) = 0;
	virtual void destroy_mesh() {};

};