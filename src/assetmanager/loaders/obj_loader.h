#pragma once

#include "../../core/core.h"
#include "../../rendercore/rendercore.h"
#include <vector>
#include <string>

struct obj_index
{
    unsigned int vertexIndex;
    unsigned int uvIndex;
    unsigned int normalIndex;
    
    bool operator<(const obj_index& r) const { return vertexIndex < r.vertexIndex; }
};

class obj_model
{
public:
    std::vector<obj_index> obj_indices;
    std::vector<vector3> vertices;
    std::vector<vector2> uvs;
    std::vector<vector3> normals;
    bool hasUVs;
    bool hasNormals;
    
    obj_model(const std::string& fileName);
    
    indexed_model to_indexed_model() const;
private:
    unsigned int find_last_vertex_index(const std::vector<const obj_index*>& indexLookup, const obj_index* currentIndex, const indexed_model& result) const;
    void create_obj_face(const std::string& line);
    
    vector2 parse_obj_vec2(const std::string& line) const;
    vector3 parse_obj_vec3(const std::string& line) const;
    obj_index parse_obj_index(const std::string& token, bool* hasUVs, bool* hasNormals) const;
};
