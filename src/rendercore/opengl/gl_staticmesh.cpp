#include "gl_staticmesh.h"
#include "rendercore/material.h"

gl_static_mesh::gl_static_mesh(const indexed_model& t_model)
	: static_mesh(t_model)
{
	m_drawCount = (int)t_model.indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERES, m_vertexArrayBuffers);

	// Vertex buffer
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, t_model.positions.size() * sizeof(t_model.positions[0]), &t_model.positions[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// TexCoords
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, t_model.positions.size() * sizeof(t_model.tex_coords[0]), &t_model.tex_coords[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Normals
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, t_model.normals.size() * sizeof(t_model.normals[0]), &t_model.normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Index buffer
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, t_model.indices.size() * sizeof(t_model.indices[0]), &t_model.indices[0], GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

gl_static_mesh::~gl_static_mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void gl_static_mesh::draw()
{
	for (const auto &mat : m_materials)
	{
		if (mat)
		{
			mat->bind();
			mat->update(transform(), transform());
		}
	}

	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
