#include "obj_loader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareOBJIndexPtr(const obj_index* a, const obj_index* b);
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);
static inline void calc_normals(indexed_model& model);

obj_model::obj_model(const std::string& fileName)
{
	hasUVs = false;
	hasNormals = false;
	std::ifstream file;
	file.open(fileName.c_str());

	std::string line;
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			unsigned int lineLength = (unsigned int)line.length();

			if (lineLength < 2)
				continue;

			const char* lineCStr = line.c_str();

			switch (lineCStr[0])
			{
			case 'v':
				if (lineCStr[1] == 't')
					this->uvs.push_back(parse_obj_vec2(line));
				else if (lineCStr[1] == 'n')
					this->normals.push_back(parse_obj_vec3(line));
				else if (lineCStr[1] == ' ' || lineCStr[1] == '\t')
					this->vertices.push_back(parse_obj_vec3(line));
				break;
			case 'f':
				create_obj_face(line);
				break;
			default: break;
			};
		}
	}
	else
	{
		std::cerr << "Unable to load mesh: " << fileName << std::endl;
	}
}

indexed_model obj_model::to_indexed_model() const
{
	indexed_model result;
	indexed_model normalModel;

	unsigned int numIndices = (unsigned int)obj_indices.size();

	std::vector<const obj_index*> indexLookup;

	for (unsigned int i = 0; i < numIndices; i++)
		indexLookup.push_back(&obj_indices[i]);

	std::sort(indexLookup.begin(), indexLookup.end(), CompareOBJIndexPtr);

	std::map<obj_index, unsigned int> normalModelIndexMap;
	std::map<unsigned int, unsigned int> indexMap;

	for (unsigned int i = 0; i < numIndices; i++)
	{
		const obj_index* currentIndex = &obj_indices[i];

		vector3 currentPosition = vertices[currentIndex->vertexIndex];
		vector2 currentTexCoord;
		vector3 currentNormal;

		if (hasUVs)
			currentTexCoord = uvs[currentIndex->uvIndex];
		else
			currentTexCoord = vector2(0, 0);

		if (hasNormals)
			currentNormal = normals[currentIndex->normalIndex];
		else
			currentNormal = vector3(0, 0, 0);

		unsigned int normalModelIndex;
		unsigned int resultModelIndex;

		//Create model to properly generate normals on
		std::map<obj_index, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
		if (it == normalModelIndexMap.end())
		{
			normalModelIndex = (unsigned int)normalModel.positions.size();

			normalModelIndexMap.insert(std::pair<obj_index, unsigned int>(*currentIndex, normalModelIndex));
			normalModel.positions.push_back(currentPosition);
			normalModel.tex_coords.push_back(currentTexCoord);
			normalModel.normals.push_back(currentNormal);
		}
		else
			normalModelIndex = it->second;

		//Create model which properly separates texture coordinates
		unsigned int previousVertexLocation = find_last_vertex_index(indexLookup, currentIndex, result);

		if (previousVertexLocation == (unsigned int)-1)
		{
			resultModelIndex = (unsigned int)result.positions.size();

			result.positions.push_back(currentPosition);
			result.tex_coords.push_back(currentTexCoord);
			result.normals.push_back(currentNormal);
		}
		else
			resultModelIndex = previousVertexLocation;

		normalModel.indices.push_back(normalModelIndex);
		result.indices.push_back(resultModelIndex);
		indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
	}

	if (!hasNormals)
	{
		calc_normals(normalModel);

		for (unsigned int i = 0; i < result.positions.size(); i++)
			result.normals[i] = normalModel.normals[indexMap[i]];
	}

	return result;
}

unsigned int obj_model::find_last_vertex_index(const std::vector<const obj_index*>& indexLookup, const obj_index* currentIndex, const indexed_model& result) const
{
	unsigned int start = 0;
	unsigned int end = (unsigned int)indexLookup.size();
	unsigned int current = (end - start) / 2 + start;
	unsigned int previous = start;

	while (current != previous)
	{
		const obj_index* testIndex = indexLookup[current];

		if (testIndex->vertexIndex == currentIndex->vertexIndex)
		{
			unsigned int countStart = current;

			for (unsigned int i = 0; i < current; i++)
			{
				const obj_index* possibleIndex = indexLookup[current - i];

				if (possibleIndex == currentIndex)
					continue;

				if (possibleIndex->vertexIndex != currentIndex->vertexIndex)
					break;

				countStart--;
			}

			for (unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
			{
				const obj_index* possibleIndex = indexLookup[current + i];

				if (possibleIndex == currentIndex)
					continue;

				if (possibleIndex->vertexIndex != currentIndex->vertexIndex)
					break;
				else if ((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex)
					&& (!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
				{
					vector3 currentPosition = vertices[currentIndex->vertexIndex];
					vector2 currentTexCoord;
					vector3 currentNormal;

					if (hasUVs)
						currentTexCoord = uvs[currentIndex->uvIndex];
					else
						currentTexCoord = vector2(0, 0);

					if (hasNormals)
						currentNormal = normals[currentIndex->normalIndex];
					else
						currentNormal = vector3(0, 0, 0);

					for (unsigned int j = 0; j < result.positions.size(); j++)
					{
						if (currentPosition == result.positions[j]
							&& ((!hasUVs || currentTexCoord == result.tex_coords[j])
								&& (!hasNormals || currentNormal == result.normals[j])))
						{
							return j;
						}
					}
				}
			}

			return -1;
		}
		else
		{
			if (testIndex->vertexIndex < currentIndex->vertexIndex)
				start = current;
			else
				end = current;
		}

		previous = current;
		current = (end - start) / 2 + start;
	}

	return -1;
}

void obj_model::create_obj_face(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->obj_indices.push_back(parse_obj_index(tokens[1], &this->hasUVs, &this->hasNormals));
    this->obj_indices.push_back(parse_obj_index(tokens[2], &this->hasUVs, &this->hasNormals));
    this->obj_indices.push_back(parse_obj_index(tokens[3], &this->hasUVs, &this->hasNormals));

    if((int)tokens.size() > 4)
    {
        this->obj_indices.push_back(parse_obj_index(tokens[1], &this->hasUVs, &this->hasNormals));
        this->obj_indices.push_back(parse_obj_index(tokens[3], &this->hasUVs, &this->hasNormals));
        this->obj_indices.push_back(parse_obj_index(tokens[4], &this->hasUVs, &this->hasNormals));
    }
}

obj_index obj_model::parse_obj_index(const std::string& token, bool* hasUVs, bool* hasNormals) const
{
    unsigned int tokenLength = (unsigned int)token.length();
    const char* tokenString = token.c_str();
    
    unsigned int vertIndexStart = 0;
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    obj_index result;
    result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    result.uvIndex = 0;
    result.normalIndex = 0;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.normalIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasNormals = true;
    
    return result;
}

vector3 obj_model::parse_obj_vec3(const std::string& line) const
{
	unsigned int tokenLength = (unsigned int)line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 2;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return vector3(x, y, z);
}

vector2 obj_model::parse_obj_vec2(const std::string& line) const
{
	unsigned int tokenLength = (unsigned int)line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 3;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return vector2(x, y);
}

static bool CompareOBJIndexPtr(const obj_index* a, const obj_index* b)
{
	return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
	unsigned int result = start;
	while (result < length)
	{
		result++;
		if (str[result] == token)
			break;
	}

	return result;
}

static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
	return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
	return (float)atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string& s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = (unsigned int)s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}

static inline void calc_normals(indexed_model& model)
{
	for (unsigned int i = 0; i < model.indices.size(); i += 3)
	{
		int i0 = model.indices[i];
		int i1 = model.indices[i + 1];
		int i2 = model.indices[i + 2];

		vector3 v1 = model.positions[i1] - model.positions[i0];
		vector3 v2 = model.positions[i2] - model.positions[i0];

		vector3 normal = vector3::cross(v1, v2).get_normalized();

		model.normals[i0] += normal;
		model.normals[i1] += normal;
		model.normals[i2] += normal;
	}

	for (unsigned int i = 0; i < model.positions.size(); i++)
		model.normals[i].normalize();
}