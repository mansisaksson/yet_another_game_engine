#pragma once
#include <GLFW/glfw3.h>
#include "../staticmesh.h"

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
	uint32_t m_drawCount;

private:
	gl_static_mesh() = delete;
	gl_static_mesh(const gl_static_mesh&) = delete;
	gl_static_mesh(gl_static_mesh&&) = delete;
	bool operator=(const gl_static_mesh&) = delete;

public:
	virtual void draw() override;

protected:
	virtual void init_mesh(const indexed_model& model) override;
	virtual void destroy_mesh() override;

};