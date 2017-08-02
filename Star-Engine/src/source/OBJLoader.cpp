#include "../header/OBJLoader.h"

Mesh* OBJLoader::loadObj(const std::string & filePath) {
	FILE * file = fopen(filePath.c_str(), "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return nullptr;
	}

	Mesh* mesh = new Mesh();
	std::vector<Vector3f> positions;
	std::vector<Vector3f> normals;
	std::vector<Vector2f> uvs;
	int i = 0;

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0) {
			Vector3f pos;
			fscanf(file, "%f %f %f\n", &(pos.x()), &(pos.y()), &(pos.z()));
			positions.push_back(pos);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vector2f uv;
			fscanf(file, "%f %f\n", &(uv.x()), &(uv.y()));
			uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vector3f normal;
			fscanf(file, "%f %f %f\n", &(normal.x()), &(normal.y()), &(normal.z()));
			normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}


			Vertex vertex1;
			vertex1.position = positions[vertexIndex[0] - 1];
			vertex1.normal = normals[normalIndex[0] - 1];
			vertex1.uv = Vector2f(uvs[uvIndex[0] - 1].x(), 1 - uvs[uvIndex[0] - 1].y());

			Vertex vertex2;
			vertex2.position = positions[vertexIndex[1] - 1];
			vertex2.normal = normals[normalIndex[1] - 1];
			vertex2.uv = Vector2f(uvs[uvIndex[1] - 1].x(), 1 - uvs[uvIndex[1] - 1].y());

			Vertex vertex3;
			vertex3.position = positions[vertexIndex[2] - 1];
			vertex3.normal = normals[normalIndex[2] - 1];
			vertex3.uv = Vector2f(uvs[uvIndex[2] - 1].x(), 1 - uvs[uvIndex[2] - 1].y());

			mesh->vertices.push_back(vertex1);
			mesh->vertices.push_back(vertex2);
			mesh->vertices.push_back(vertex3);

			// How is this even remotely correct??
			mesh->indices.push_back(i++);
			mesh->indices.push_back(i++);
			mesh->indices.push_back(i++);
		}
	}

	return mesh;
}