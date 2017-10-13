#include "Material.h"
#include "CubeMap.h"
#include "Texture.h"

void Material::setTexture(const std::string & uniformName, Texture* texture){
	_textreMap[uniformName] = texture;
}

void Material::setCubeMap(const std::string & uniformName, CubeMap* cubeMap){
	_cubeMapMap[uniformName] = cubeMap;
}

// TODO: Iterate over all found uniforms and set the unused ones to default values
void Material::updateUniforms() {
	updateUniforms(shader);
}

void Material::updateUniforms(Shader* shader){
	for (std::unordered_map<std::string, SuperObject*>::const_iterator it = _uniformMap.begin(); it != _uniformMap.end(); ++it) {
		it->second->update(shader);
	}
}

void Material::bindTextures() {
	bindTextures(shader);
}

void Material::bindTextures(Shader* shader){
	// Textures
	for (std::unordered_map<std::string, Texture*>::const_iterator it = _textreMap.begin(); it != _textreMap.end(); ++it) {
		if(shader->textures.find(it->first) != shader->textures.end())
			it->second->bind(shader->textures[it->first].unit);
	}

	// CubeMaps
	for (std::unordered_map<std::string, CubeMap*>::const_iterator it = _cubeMapMap.begin(); it != _cubeMapMap.end(); ++it) {
		if (shader->textures.find(it->first) != shader->textures.end())
			it->second->bind(shader->textures[it->first].unit);
	}
}

Texture* Material::getTexture(const std::string & uniformName){
	return _textreMap[uniformName];
}

CubeMap* Material::getCubeMap(const std::string & uniformName){
	return _cubeMapMap[uniformName];
}