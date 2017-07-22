#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"


class Material {
public:
	Material(Shader* shader): shader(shader) {}

	Shader* shader;
};

#endif /* defined(MATERIAL_H) */
