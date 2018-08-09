#ifndef MESH_SERIALIZER_H
#define MESH_SERIALIZER_H

class Mesh;

#include <string>

class MeshSerializer
{
public:
	static void write(const std::string& pathName, Mesh* mesh);
	static void read(const std::string & pathName, Mesh* mesh);
};

#endif // MESH_SERIALIZER_H
