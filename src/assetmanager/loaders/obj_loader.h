// OBJ_Loader.h - A Single Header OBJ Model Loader

#pragma once
#include "core/core.h"
#include "rendercore/staticmesh.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>

namespace obj
{
	struct material
	{
		std::string name = "";
		vector3 Ka;				// Ambient Color
		vector3 Kd;				// Diffuse Color
		vector3 Ks;				// Specular Color
		float Ns = 0.f;			// Specular Exponent
		float Ni = 0.f;			// Optical Density
		float d = 0.f;			// Dissolve
		int illum = 0;			// Illumination
		std::string map_Ka;		// Ambient Texture Map
		std::string map_Kd;		// Diffuse Texture Map
		std::string map_Ks;		// Specular Texture Map
		std::string map_Ns;		// Specular Hightlight Map
		std::string map_d;		// Alpha Texture Map
		std::string map_bump;	// Bump Map
	};

	// Namespace: Math
	//
	// Description: The namespace that holds all of the math
	//	functions need for OBJL
	namespace objl_math
	{
		// Angle between 2 Vector3 Objects
		float angle_between_v3(const vector3 &a, const vector3 &b)
		{
			const float angle = vector3::dot(a, b) / (a.length() * b.length());
			return acosf(angle);
		}

		// Projection Calculation of a onto b
		vector3 proj_v3(const vector3 &a, const vector3 &b)
		{
			const vector3 bn = b / b.length();
			return bn * vector3::dot(a, bn);
		}
	}

	// Namespace: Algorithm
	//
	// Description: The namespace that holds all of the
	// Algorithms needed for OBJL
	namespace algorithm
	{
		// A test to see if P1 is on the same side as P2 of a line segment ab
		bool same_side(const vector3 &p1, const vector3 &p2, const vector3 &a, const vector3 &b)
		{
			const vector3 cp1 = vector3::cross(b - a, p1 - a);
			const vector3 cp2 = vector3::cross(b - a, p2 - a);
			return vector3::dot(cp1, cp2) >= 0;
		}

		// Generate a cross produect normal for a triangle
		vector3 gen_tri_normal(const vector3 &t1, const vector3 &t2, const vector3 &t3)
		{
			const vector3 u = t2 - t1;
			const vector3 v = t3 - t1;
			const vector3 normal = vector3::cross(u, v);
			return normal;
		}

		// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
		bool in_triangle(const vector3 &point, const vector3 &tri1, const vector3 &tri2, const vector3 &tri3)
		{
			// Test to see if it is within an infinite prism that the triangle outlines.
			const bool within_tri_prisim = same_side(point, tri1, tri2, tri3) 
				&& same_side(point, tri2, tri1, tri3)
				&& same_side(point, tri3, tri1, tri2);
			
			if (!within_tri_prisim) // If it isn't it will never be on the triangle
				return false;
			
			const vector3 n = gen_tri_normal(tri1, tri2, tri3); // Calulate Triangle's Normal
			const vector3 proj = objl_math::proj_v3(point, n); // Project the point onto this normal
			return math::nearly_zero(proj.length()); // If the distance from the triangle to the point is 0 it lies on the triangle
		}

		// Split a String into a string array at a given token
		inline std::vector<std::string> split(const std::string &in, const std::string &token)
		{
			std::vector<std::string> out;
			std::string temp;

			for (int i = 0; i < int(in.size()); i++)
			{
				const std::string test = in.substr(i, token.size());

				if (test == token)
				{
					if (!temp.empty())
					{
						out.push_back(temp);
						temp.clear();
						i += (int)token.size() - 1;
					}
					else
					{
						out.push_back("");
					}
				}
				else if (i + token.size() >= in.size())
				{
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				}
				else
				{
					temp += in[i];
				}
			}

			return out;
		}

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string &in)
		{
			const size_t token_start = in.find_first_not_of(" \t");
			const size_t space_start = in.find_first_of(" \t", token_start);
			const size_t tail_start  = in.find_first_not_of(" \t", space_start);
			const size_t tail_end	 = in.find_last_not_of(" \t");
			if (tail_start != std::string::npos && tail_end != std::string::npos)
			{
				return in.substr(tail_start, tail_end - tail_start + 1);
			}
			else if (tail_start != std::string::npos)
			{
				return in.substr(tail_start);
			}
			return "";
		}

		// Get first token of string
		inline std::string first_token(const std::string &in)
		{
			if (!in.empty())
			{
				size_t token_start = in.find_first_not_of(" \t");
				size_t token_end = in.find_first_of(" \t", token_start);
				if (token_start != std::string::npos && token_end != std::string::npos)
				{
					return in.substr(token_start, token_end - token_start);
				}
				else if (token_start != std::string::npos)
				{
					return in.substr(token_start);
				}
			}
			return "";
		}

		// Get element at given index position
		template <class T>
		inline const T & get_element(const std::vector<T> &elements, const std::string &index)
		{
			int idx = std::stoi(index);
			if (idx < 0)
				idx = int(elements.size()) + idx;
			else
				idx--;
			return elements[idx];
		}
	}

	class obj_loader
	{
	public:
		static std::vector<indexed_model> load_obj(const std::string &t_path)
		{
			if (t_path.substr(t_path.size() - 4, 4) != ".obj") // If the file is not an .obj file return false
			{
				log::error("obj_loader", "tried loading non obj file: %s", t_path.c_str());
				return {};
			}

			std::ifstream file(t_path);
			if (!file.is_open())
			{
				log::error("obj_loader", "Failed to open file: %s", t_path.c_str());
				return {};
			}

			std::vector<indexed_model> out_meshes;

			struct file_data
			{
				std::vector<vector3> positions;
				std::vector<vector2> tex_coords;
				std::vector<vector3> normals;
				std::vector<std::string> mat_names;
			} file_data;

			struct mesh_data
			{
				std::string mesh_name;
				std::string material_name;
				std::vector<vector3> positions;
				std::vector<vector2> tex_coords;
				std::vector<vector3> normals;
				std::vector<uint32_t> indices;
			} current_mesh;

			bool listening = false;
			std::string current_line;
			while (std::getline(file, current_line))
			{
				// Generate a Mesh Object or Prepare for an object to be created
				if (algorithm::first_token(current_line) == "o" || algorithm::first_token(current_line) == "g" || current_line[0] == 'g')
				{
					if (!current_mesh.indices.empty() && !current_mesh.positions.empty()) // Generate the mesh to put into the array
					{
						out_meshes.push_back(indexed_model{
							current_mesh.mesh_name,
							current_mesh.positions,
							current_mesh.tex_coords,
							current_mesh.normals,
							current_mesh.indices,
							current_mesh.material_name
						});
						current_mesh = mesh_data();
					}
					
					if (algorithm::first_token(current_line) == "o" || algorithm::first_token(current_line) == "g")
					{
						current_mesh.mesh_name = algorithm::tail(current_line);
					}
					else
					{
						current_mesh.mesh_name = "unnamed";
					}
				}

				// Generate a Position
				if (algorithm::first_token(current_line) == "v")
				{
					const std::vector<std::string> str_pos = algorithm::split(algorithm::tail(current_line), " ");
					const vector3 vert_pos = { std::stof(str_pos[0]), std::stof(str_pos[1]), std::stof(str_pos[2]) };
					file_data.positions.push_back(vert_pos);
				}

				// Generate a Texture Coordinate
				if (algorithm::first_token(current_line) == "vt")
				{
					const std::vector<std::string> str_tex = algorithm::split(algorithm::tail(current_line), " ");
					const vector2 vert_tex = { std::stof(str_tex[0]), std::stof(str_tex[1]) };
					file_data.tex_coords.push_back(vert_tex);
				}

				// Generate a Normal;
				if (algorithm::first_token(current_line) == "vn")
				{
					const std::vector<std::string> str_normal = algorithm::split(algorithm::tail(current_line), " ");
					const vector3 vert_normal = { std::stof(str_normal[0]), std::stof(str_normal[1]), std::stof(str_normal[2]) };
					file_data.normals.push_back(vert_normal);
				}

				// Generate a Face (vertices & indices)
				if (algorithm::first_token(current_line) == "f")
				{
					// Generate the vertices
					const std::vector<vertex> verts = gen_vertices_from_raw_obj(file_data.positions, file_data.tex_coords, file_data.normals, current_line);
					current_mesh.positions.reserve(current_mesh.positions.size() + verts.size());
					current_mesh.tex_coords.reserve(current_mesh.tex_coords.size() + verts.size());
					current_mesh.normals.reserve(current_mesh.normals.size() + verts.size());
					for (int i = 0; i < int(verts.size()); i++)
					{
						current_mesh.positions.push_back(verts[i].position);
						current_mesh.tex_coords.push_back(verts[i].tex_coord);
						current_mesh.normals.push_back(verts[i].normal);
					}

					const std::vector<uint32_t> indices = triangulate_vertices(verts);
					current_mesh.indices.reserve(current_mesh.indices.size() + indices.size());
					for (int i = 0; i < int(indices.size()); i++)
					{
						const uint32_t indnum = (uint32_t)((current_mesh.positions.size()) - verts.size()) + indices[i];
						current_mesh.indices.push_back(indnum);
					}
				}

				// Get Mesh Material Name
				if (algorithm::first_token(current_line) == "usemtl")
				{
					// Create new Mesh, if Material changes within a group
					if (!current_mesh.indices.empty() && !current_mesh.positions.empty())
					{
						out_meshes.push_back(indexed_model{
							current_mesh.mesh_name,
							current_mesh.positions,
							current_mesh.tex_coords,
							current_mesh.normals,
							current_mesh.indices,
							current_mesh.material_name
							});
						current_mesh = mesh_data();
					}

					current_mesh.material_name = algorithm::tail(current_line);
				}

				// Load Materials
				if (algorithm::first_token(current_line) == "mtllib")
				{
					// TODO: not really interested in material properties, only which material goes where

					//// Generate a path to the material file
					//const std::vector<std::string> temp = algorithm::split(t_path, "/");

					//std::string pathtomat = "";

					//if (temp.size() != 1)
					//{
					//	for (int i = 0; i < temp.size() - 1; i++)
					//	{
					//		pathtomat += temp[i] + "/";
					//	}
					//}
					//pathtomat += algorithm::tail(current_line);

					//// Load Materials
					//load_materials(pathtomat); 
				}
			}

			// Deal with last mesh
			if (!current_mesh.indices.empty() && !current_mesh.positions.empty())
			{
				out_meshes.push_back(indexed_model{
					current_mesh.mesh_name,
					current_mesh.positions,
					current_mesh.tex_coords,
					current_mesh.normals,
					current_mesh.indices,
					current_mesh.material_name
				});
			}

			file.close();

			return out_meshes;
		}

	private:
		// Generate vertices from a list of positions, 
		//	tcoords, normals and a face line
		static std::vector<vertex> gen_vertices_from_raw_obj(
			const std::vector<vector3> &t_positions, 
			const std::vector<vector2>& t_tex_coords, 
			const std::vector<vector3>& t_normals, 
			const std::string &t_current_line)
		{
			const std::vector<std::string> str_face = algorithm::split(algorithm::tail(t_current_line), " ");

			bool no_normal = false;
			std::vector<vertex> out_vertices;

			// For every given vertex do this
			for (int i = 0; i < int(str_face.size()); i++)
			{
				const std::vector<std::string> str_vert = algorithm::split(str_face[i], "/");

				// See What type the vertex is.
				const int vtype = [&]()
				{
					if (str_vert.size() == 1) // Check for just position - v1
						return 1; // Only position

					if (str_vert.size() == 2) // Check for position & texture - v1/vt1
						return 2; // Position & Texture

					// Check for Position, Texture and Normal - v1/vt1/vn1
					// or if Position and Normal - v1//vn1
					if (str_vert.size() == 3)
					{
						if (str_vert[1] != "")
							return 4; // Position, Texture, and Normal
						else
							return 3; // Position & Normal
					}

					return 1;
				}();

				const vertex vert = [&]()->vertex {
					switch (vtype)
					{
					case 1: // P
						no_normal = true;
						return { algorithm::get_element(t_positions, str_vert[0]), vector2(0, 0), vector3::zero };
					case 2: // P/T
						no_normal = true;
						return { algorithm::get_element(t_positions, str_vert[0]), algorithm::get_element(t_tex_coords, str_vert[1]), vector3::zero };
					case 3: // P//N
						return { algorithm::get_element(t_positions, str_vert[0]), vector2(0, 0), algorithm::get_element(t_normals, str_vert[2]) };
					case 4: // P/T/N
						return { algorithm::get_element(t_positions, str_vert[0]), algorithm::get_element(t_tex_coords, str_vert[1]), algorithm::get_element(t_normals, str_vert[2]) };
					default:
						return vertex();
					}
				}();

				out_vertices.push_back(vert);
			}

			// take care of missing normals
			// these may not be truly acurate but it is the 
			// best they get for not compiling a mesh with normals	
			if (no_normal)
			{
				const vector3 A = out_vertices[0].position - out_vertices[1].position;
				const vector3 B = out_vertices[2].position - out_vertices[1].position;
				const vector3 normal = vector3::cross(A, B);

				for (size_t i = 0; i < out_vertices.size(); i++)
					out_vertices[i].normal = normal;
			}

			return out_vertices;
		}

		static std::vector<uint32_t> triangulate_vertices(const std::vector<vertex>& verts)
		{
			if (verts.size() < 3) // If there are 2 or less verts, no triangle can be created,
				return {};
			
			if (verts.size() == 3) // If it is a triangle no need to calculate it
				return { 0, 1, 2 };

			// Create a list of vertices
			std::vector<vertex> verts_cpy = verts;
			std::vector<uint32_t> out_indices;
			out_indices.reserve(verts_cpy.size() / 3);

			while (true)
			{
				for (size_t i = 0; i < verts_cpy.size(); i++)
				{
					vertex &prev = i == 0 ? verts_cpy[verts_cpy.size() - 1] : verts_cpy[i - 1];
					vertex &current = verts_cpy[i];
					vertex &next = (i == verts_cpy.size() - 1) ? verts_cpy[0] : verts_cpy[i + 1];

					// Check to see if there are only 3 verts left
					// if so this is the last triangle
					if (verts_cpy.size() == 3)
					{
						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(verts_cpy.size()); j++)
						{
							if (verts[j].position == current.position)
								out_indices.push_back(j);
							if (verts[j].position == prev.position)
								out_indices.push_back(j);
							if (verts[j].position == next.position)
								out_indices.push_back(j);
						}

						verts_cpy.clear();
						break;
					}

					if (verts_cpy.size() == 4)
					{
						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(verts.size()); j++)
						{
							if (verts[j].position == current.position)
								out_indices.push_back(j);
							if (verts[j].position == prev.position)
								out_indices.push_back(j);
							if (verts[j].position == next.position)
								out_indices.push_back(j);
						}

						vector3 tempVec;
						for (int j = 0; j < int(verts_cpy.size()); j++)
						{
							if (verts_cpy[j].position != current.position
								&& verts_cpy[j].position != prev.position
								&& verts_cpy[j].position != next.position)
							{
								tempVec = verts_cpy[j].position;
								break;
							}
						}

						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(verts.size()); j++)
						{
							if (verts[j].position == prev.position)
								out_indices.push_back(j);
							if (verts[j].position == next.position)
								out_indices.push_back(j);
							if (verts[j].position == tempVec)
								out_indices.push_back(j);
						}

						verts_cpy.clear();
						break;
					}

					// If Vertex is not an interior vertex
					const float angle = objl_math::angle_between_v3(prev.position - current.position, next.position - current.position) * (180.f / math::pi());
					if (angle <= 0 && angle >= 180)
						continue;

					// If any vertices are within this triangle
					bool inTri = false;
					for (int j = 0; j < int(verts.size()); j++)
					{
						if (algorithm::in_triangle(verts[j].position, prev.position, current.position, next.position)
							&& verts[j].position != prev.position
							&& verts[j].position != current.position
							&& verts[j].position != next.position)
						{
							inTri = true;
							break;
						}
					}
					if (inTri)
						continue;

					// Create a triangle from pCur, pPrev, pNext
					for (int j = 0; j < int(verts.size()); j++)
					{
						if (verts[j].position == current.position)
							out_indices.push_back(j);
						if (verts[j].position == prev.position)
							out_indices.push_back(j);
						if (verts[j].position == next.position)
							out_indices.push_back(j);
					}

					// Delete pCur from the list
					for (int j = 0; j < int(verts_cpy.size()); j++)
					{
						if (verts_cpy[j].position == current.position)
						{
							verts_cpy.erase(verts_cpy.begin() + j);
							break;
						}
					}

					// reset i to the start -1 since loop will add 1 to it
					i = -1;
				}
				
				if (out_indices.size() == 0) // if no triangles were created
					break;
				
				if (verts_cpy.size() == 0) // if no more vertices
					break;
			}

			return out_indices;
		}

		// Load Materials from .mtl file
		static std::vector<material> load_materials(const std::string &path)
		{
			if (path.substr(path.size() - 4, path.size()) != ".mtl") // If the file is not a material file, return
				return {};

			std::ifstream file(path);
			if (!file.is_open())
				return {};

			std::vector<material> out_materials;

			material temp_material;
			bool listening = false;
			std::string current_line;
			while (std::getline(file, current_line))
			{
				// new material and material name
				if (algorithm::first_token(current_line) == "newmtl")
				{
					if (!listening)
					{
						listening = true;

						if (current_line.size() > 7)
						{
							temp_material.name = algorithm::tail(current_line);
						}
						else
						{
							temp_material.name = "none";
						}
					}
					else
					{
						out_materials.push_back(temp_material);
						temp_material = material();

						if (current_line.size() > 7)
						{
							temp_material.name = algorithm::tail(current_line);
						}
						else
						{
							temp_material.name = "none";
						}
					}
				}
				// Ambient Color
				if (algorithm::first_token(current_line) == "Ka")
				{
					const std::vector<std::string> temp = algorithm::split(algorithm::tail(current_line), " ");
					if (temp.size() != 3)
						continue;

					temp_material.Ka.x = std::stof(temp[0]);
					temp_material.Ka.y = std::stof(temp[1]);
					temp_material.Ka.z = std::stof(temp[2]);
				}
				// Diffuse Color
				if (algorithm::first_token(current_line) == "Kd")
				{
					const std::vector<std::string> temp = algorithm::split(algorithm::tail(current_line), " ");

					if (temp.size() != 3)
						continue;

					temp_material.Kd.x = std::stof(temp[0]);
					temp_material.Kd.y = std::stof(temp[1]);
					temp_material.Kd.z = std::stof(temp[2]);
				}
				// Specular Color
				if (algorithm::first_token(current_line) == "Ks")
				{
					const std::vector<std::string> temp = algorithm::split(algorithm::tail(current_line), " ");

					if (temp.size() != 3)
						continue;

					temp_material.Ks.x = std::stof(temp[0]);
					temp_material.Ks.y = std::stof(temp[1]);
					temp_material.Ks.z = std::stof(temp[2]);
				}
				// Specular Exponent
				if (algorithm::first_token(current_line) == "Ns")
				{
					temp_material.Ns = std::stof(algorithm::tail(current_line));
				}
				// Optical Density
				if (algorithm::first_token(current_line) == "Ni")
				{
					temp_material.Ni = std::stof(algorithm::tail(current_line));
				}
				// Dissolve
				if (algorithm::first_token(current_line) == "d")
				{
					temp_material.d = std::stof(algorithm::tail(current_line));
				}
				// Illumination
				if (algorithm::first_token(current_line) == "illum")
				{
					temp_material.illum = std::stoi(algorithm::tail(current_line));
				}
				// Ambient Texture Map
				if (algorithm::first_token(current_line) == "map_Ka")
				{
					temp_material.map_Ka = algorithm::tail(current_line);
				}
				// Diffuse Texture Map
				if (algorithm::first_token(current_line) == "map_Kd")
				{
					temp_material.map_Kd = algorithm::tail(current_line);
				}
				// Specular Texture Map
				if (algorithm::first_token(current_line) == "map_Ks")
				{
					temp_material.map_Ks = algorithm::tail(current_line);
				}
				// Specular Hightlight Map
				if (algorithm::first_token(current_line) == "map_Ns")
				{
					temp_material.map_Ns = algorithm::tail(current_line);
				}
				// Alpha Texture Map
				if (algorithm::first_token(current_line) == "map_d")
				{
					temp_material.map_d = algorithm::tail(current_line);
				}
				// Bump Map
				if (algorithm::first_token(current_line) == "map_Bump" || algorithm::first_token(current_line) == "map_bump" || algorithm::first_token(current_line) == "bump")
				{
					temp_material.map_bump = algorithm::tail(current_line);
				}
			}

			// Push Back loaded Material
			out_materials.push_back(temp_material);
			return out_materials;
		}
	};
}
