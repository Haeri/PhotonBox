#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"

class Material {
public:
	Material(Shader* shader): shader(shader) {}
	Material(Shader* shader, Texture* texture): shader(shader), texture(texture){}

	Shader* shader;
	Texture* texture;
};

#endif /* defined(MATERIAL_H) */
