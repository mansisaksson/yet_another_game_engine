#include "gl_staticmesh.h"
#include "rendercore/material.h"
#include "rendercore/viewport.h"

gl_static_mesh::gl_static_mesh(const std::vector<indexed_model>& t_models)
	: static_mesh(t_models)
{
	gl_meshes_data.reserve(t_models.size());
	for (const auto &model : t_models)
	{
		gl_mesh_data mesh_data;
		mesh_data.draw_count = (int)model.indices.size();

		glGenVertexArrays(1, &mesh_data.vertex_array_object);
		glBindVertexArray(mesh_data.vertex_array_object);

		glGenBuffers(NUM_BUFFERES, mesh_data.vertex_array_buffers);

		// Vertex buffer
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh_data.vertex_array_buffers[POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// TexCoords
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh_data.vertex_array_buffers[TEXCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.tex_coords[0]), &model.tex_coords[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Normals
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh_data.vertex_array_buffers[NORMAL_VB]);
			glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Index buffer
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data.vertex_array_buffers[INDEX_VB]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);
		}

		glBindVertexArray(0);

		gl_meshes_data.push_back(mesh_data);
	}
}

gl_static_mesh::~gl_static_mesh()
{
	for (const auto &mesh_data : gl_meshes_data)
	{
		glDeleteVertexArrays(1, &mesh_data.vertex_array_object);
	}
}

void gl_static_mesh::draw(const transform& t_mesh_transform, const viewport* t_viewport)
{
	for (size_t i = 0; i < gl_meshes_data.size(); i++)
	{
		const auto mat = m_materials[i];

		if (mat)
		{
			mat->set_uniform_matrix4x4("view_transform", t_viewport->get_view_projection());
			mat->set_uniform_matrix4x4("object_transform", t_mesh_transform.to_matrix());
			mat->apply_material();
		}

		glBindVertexArray(gl_meshes_data[i].vertex_array_object);
		glDrawElements(GL_TRIANGLES, gl_meshes_data[i].draw_count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
