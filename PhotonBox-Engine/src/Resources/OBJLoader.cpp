#include "OBJLoader.h"
#include <fstream>
#include <string>
#include <map>
#include "../Core/Util.h"


struct OBJIndex {
	int position;
	int uv;
	int normal;
};

void parseFace(const std::string& token, std::vector<OBJIndex>& indexList, std::map<OBJIndex, int>& faceIndexMap, int cnt) {

	int p = -1, u = -1, n = -1;
	OBJIndex obj;

	std::vector<std::string> tokens;
	Util::split(token, "/", tokens);
	
	if (tokens.size() > 0) {
		p = std::stoi(tokens[0]);
		if (p != -1) {
			obj.position = p - 1;
		}

		if (tokens.size() > 1) {
			u = std::stoi(tokens[1]);
			if (u != -1) {
				obj.uv = u - 1;
			}
		
			if (tokens.size() > 2) {
				n = std::stoi(tokens[2]);
				if (n != -1) {
					obj.normal = n - 1;
				}
			}
		}
	}

	/*
	if (faceIndexMap.find(obj) != faceIndexMap.end()) {
		 
	}else {

	}
	*/
	
	indexList.push_back(obj);
}

Vector3f calculateTangent(Vector3f p1, Vector3f p2, Vector3f p3, Vector2f uv1, Vector2f uv2, Vector2f uv3, Vector3f normal) {	
	Vector3f tangent;

	Vector3f edge1 = p2 - p1;
	Vector3f edge2 = p3 - p1;
	Vector2f deltaUV1 = uv2 - uv1;
	Vector2f deltaUV2 = uv3 - uv1;
	//Vector3f normal = Vector3f::cross(edge1, edge2);

	float f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

	tangent.x() = f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x());
	tangent.y() = f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y());
	tangent.z() = f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z());
	tangent.normalize();
	/*
	bitangent.x() = f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x());
	bitangent.y() = f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y());
	bitangent.z() = f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z());
	bitangent.normalize();
	*/
	return tangent;
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
	std::map<OBJIndex, int> faceIndexMap;
	int cnt = 0;

	while (std::getline(file, line)) {
		
		tokens.clear();
		Util::split(line, " ", tokens);
		
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
			uv.y() = 1 - std::stof(tokens[2]);
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
				parseFace(tokens[1 + i], indices, faceIndexMap, cnt++);
				parseFace(tokens[2 + i], indices, faceIndexMap, cnt++);
				parseFace(tokens[3 + i], indices, faceIndexMap, cnt++);
			}	
		}
	}

	// Create Indexed Model 
	for (size_t i = 0; i <= indices.size() - 3; i += 3)
	{
		Vector3f tangent = calculateTangent(
			positions[indices[i].position], positions[indices[i + 1].position], positions[indices[i + 2].position],
			uvs[indices[i].uv], uvs[indices[i + 1].uv], uvs[indices[i + 2].uv], normals[indices[i].normal]);

		mesh->vertices.push_back(Vertex(
			positions[indices[i].position],
			normals[indices[i].normal],
			Vector3f::ZERO,
			uvs[indices[i].uv],
			tangent
		));
		mesh->vertices.push_back(Vertex(
			positions[indices[i+1].position],
			normals[indices[i+1].normal],
			Vector3f::ZERO,
			uvs[indices[i+1].uv],
			tangent
		));
		mesh->vertices.push_back(Vertex(
			positions[indices[i+2].position],
			normals[indices[i+2].normal],
			Vector3f::ZERO,
			uvs[indices[i+2].uv],
			tangent
		));

		mesh->indices.push_back(i);
		mesh->indices.push_back(i+1);
		mesh->indices.push_back(i+2);
	}

	return mesh;
}
