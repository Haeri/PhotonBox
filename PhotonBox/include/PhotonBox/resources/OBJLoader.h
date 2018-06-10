#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

class Mesh;

class OBJLoader
{
public:
	static void loadObj(const std::string& filePath, Mesh* mesh);
};

#endif /* defined(OBJ_LOADER_H) */
