#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "ForwardShader.h"
#include "Texture.h"

class Material {
public:
	Material(Shader* shader): shader(shader) {}
	Material(Shader* shader, Texture* texture): shader(shader), texture(texture){}
	Material(ForwardShader* forwardShader) : forwardShader(forwardShader) {}
	Material(ForwardShader* forwardShader, Texture* texture) : forwardShader(forwardShader), texture(texture) {}

	Shader* shader;
	ForwardShader* forwardShader;
	Texture* texture;
};

#endif /* defined(MATERIAL_H) */
