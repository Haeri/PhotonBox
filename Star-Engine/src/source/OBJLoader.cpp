#include "../header/OBJLoader.h"
#include <fstream>
#include <string>


struct OBJIndex {
	int position;
	int uv;
	int normal;
};

void split(const std::string& line, const std::string& delimiter, std::vector<std::string>& output) {
	std::string s = line;
	size_t pos = 0;
	std::string token;

	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		output.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	output.push_back(s);
}

void parseFace(const std::string& token, std::vector<OBJIndex>& indexList) {

	int p = -1, u = -1, n = -1;
	OBJIndex obj;

	std::vector<std::string> tokens;
	split(token, "/", tokens);

	p = std::stoi(tokens[0]);
	u = std::stoi(tokens[1]);
	n = std::stoi(tokens[2]);

	if (p != -1) {
		obj.position = p - 1;
	}
	if (u != -1) {
		obj.uv = u - 1;
	}
	if (n != -1) {
		obj.normal = n - 1;
	}

	indexList.push_back(obj);
}

Mesh* OBJLoader::loadObj(const std::string & filePath) {
	std::ifstream file(filePath);
	std::string line;
	std::vector<std::string> tokens;

	Mesh* mesh = new Mesh();
	std::vector<Vector3f> positions;
	std::vector<Vector3f> normals;
	std::vector<Vector2f> uvs;
	std::vector<OBJIndex> indices;

	while (std::getline(file, line)) {
		
		tokens.clear();
		split(line, " ", tokens);
		
		if (tokens[0].compare("") == 0 || tokens[0].compare(0, 1, "#") == 0) {
			continue;
		}
		else if (tokens[0].compare("v") == 0) {
			Vector3f pos;
			pos.x() = std::stof(tokens[1]);
			pos.y() = std::stof(tokens[2]);
			pos.z() = std::stof(tokens[3]);
			positions.push_back(pos);
		}
		else if (tokens[0].compare("vt") == 0) {
			Vector2f uv;
			uv.x() = std::stof(tokens[1]);
			uv.y() = std::stof(tokens[2]);
			uvs.push_back(uv);
		}
		else if (tokens[0].compare("vn") == 0) {
			Vector3f normal;
			normal.x() = std::stof(tokens[1]);
			normal.y() = std::stof(tokens[2]);
			normal.z() = std::stof(tokens[3]);
			normals.push_back(normal);
		}
		else if (tokens[0].compare("f") == 0) {
			for (int i = 0; i < tokens.size()-3; ++i) {
				parseFace(tokens[1 + i], indices);
				parseFace(tokens[2 + i], indices);
				parseFace(tokens[3 + i], indices);
			}	
		}
	}

	// Create Indexed Model 
	for (size_t i = 0; i < indices.size(); ++i)
	{
		mesh->vertices.push_back(Vertex(
			positions[indices[i].position], 
			normals[indices[i].normal], 
			Vector3f::ZERO, 
			uvs[indices[i].uv]
		));

		mesh->indices.push_back(i);
	}

	return mesh;
}