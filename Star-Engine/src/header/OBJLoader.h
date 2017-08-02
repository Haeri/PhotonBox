#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "Mesh.h"

class OBJLoader {
public:
	static Mesh* loadObj(const std::string& filePath);
};

#endif /* defined(OBJ_LOADER_H) */
