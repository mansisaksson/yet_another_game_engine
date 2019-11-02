#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "rendercore/staticmesh.h"

class gl_static_mesh : public static_mesh
{
private:
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,

		NUM_BUFFERES
	};

	struct gl_mesh_data
	{
		GLuint vertex_array_object;
		GLuint vertex_array_buffers[NUM_BUFFERES];
		uint32_t draw_count = 0;
	};
	std::vector<gl_mesh_data> gl_meshes_data;

private:
	gl_static_mesh() = delete;
	gl_static_mesh(const gl_static_mesh&) = delete;
	gl_static_mesh(gl_static_mesh&&) = delete;
	bool operator=(const gl_static_mesh&) = delete;

public:
	gl_static_mesh(const std::vector<indexed_model>& t_models);
	~gl_static_mesh();

	virtual void draw(const transform& t_mesh_transform, const viewport* t_viewport) override;

};