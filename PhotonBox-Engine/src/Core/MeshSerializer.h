#ifndef MESH_SERIALIZER_H
#define MESH_SERIALIZER_H

class Mesh;
#include "../Resources/Vertex.h"

class MeshSerializer {
public:
	static void write(const std::string& pathName, Mesh* mesh);
	static Mesh* read(const std::string&  pathName);
};

#endif // MESH_SERIALIZER_H