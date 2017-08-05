#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "Mesh.h"

class OBJLoader {
public:
	static Mesh* loadObj(const std::string& filePath);
};

#endif /* defined(OBJ_LOADER_H) */
