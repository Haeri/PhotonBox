#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "ForwardShader.h"
#include "Texture.h"

class Material {
public:
	Material(Shader* shader): shader(shader) {}
	Material(Shader* shader, Texture* texture): shader(shader), albedoMap(texture) {}
	Material(Shader* shader, Texture* texture, Texture* normalMap) : shader(shader), albedoMap(texture), normalMap(normalMap) {}
	Material(ForwardShader* forwardShader) : forwardShader(forwardShader) {}
	Material(ForwardShader* forwardShader, Texture* texture) : forwardShader(forwardShader), albedoMap(texture) {}
	Material(ForwardShader* forwardShader, Texture* texture, Texture* normalMap) : forwardShader(forwardShader), albedoMap(texture), normalMap(normalMap) {}

	Shader* shader;
	ForwardShader* forwardShader;
	Texture* albedoMap;
	Texture* normalMap;
	Texture* specularMap;
	Texture* aoMap;
	Texture* emissionMap;
};

#endif /* defined(MATERIAL_H) */
