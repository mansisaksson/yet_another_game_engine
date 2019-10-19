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

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERES];
	uint32_t m_drawCount = 0;

private:
	gl_static_mesh() = delete;
	gl_static_mesh(const gl_static_mesh&) = delete;
	gl_static_mesh(gl_static_mesh&&) = delete;
	bool operator=(const gl_static_mesh&) = delete;

public:
	gl_static_mesh(const indexed_model& t_model);
	~gl_static_mesh();

	virtual void draw() override;

};