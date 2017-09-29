#include "Material.h"
#include "CubeMap.h"
#include "Texture.h"

/*
void Material::setUniform(const std::string & uniformName, int value){
	_uniformMap[uniformName] = new IntObject(value, shader, uniformName);
}

void Material::setUniform(const std::string & uniformName, float value){
	_uniformMap[uniformName] = new FloatObject(value, shader, uniformName);
}

void Material::setUniform(const std::string & uniformName, bool value){
	_uniformMap[uniformName] = new BoolObject(value, shader, uniformName);
}

void Material::setUniform(const std::string & uniformName, Vector2f value){
	_uniformMap[uniformName] = new Vec2Object(value, shader, uniformName);
}

void Material::setUniform(const std::string & uniformName, Vector3f value){
	_uniformMap[uniformName] = new Vec3Object(value, shader, uniformName);
}

void Material::setUniform(const std::string & uniformName, Vector4f value){
	_uniformMap[uniformName] = new Vec4Object(value, shader, uniformName);
}

void Material::setUniform(const std::string & uniformName, Matrix4f value){
	_uniformMap[uniformName] = new Mat4Object(value, shader, uniformName);
}
*/

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
	for (std::unordered_map<std::string, byte*>::const_iterator it = _uniformMap.begin(); it != _uniformMap.end(); ++it) {
		float f = 0;
		shader->setUniform(it->first, from_bytes(*(it->second), f));
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
