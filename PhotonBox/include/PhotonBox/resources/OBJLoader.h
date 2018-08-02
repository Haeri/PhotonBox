#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

class Mesh;

#include <string>

class OBJLoader
{
public:
	static void loadObj(const std::string& filePath, Mesh* mesh);
};

#endif // OBJ_LOADER_H
