#include "PhotonBox/resource/Material.h"

#include "PhotonBox/resource/FrameBuffer.h"
#include "PhotonBox/resource/CubeMap.h"
#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/resource/Texture.h"
#include "PhotonBox/resource/ImageBuffer.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Material::~Material()
{
	for (std::unordered_map<std::string, SuperObject*>::iterator it = _uniformMap.begin(); it != _uniformMap.end(); ++it)
	{
		delete it->second;
	}
	_uniformMap.clear();
}

void Material::setImageBuffer(const std::string& uniformName, ImageBuffer* image)
{
	_imageBufferMap[uniformName] = image;
}

void Material::updateUniforms()
{
	updateUniforms(_shader);
}

void Material::updateUniforms(Shader* shader)
{
	for (std::unordered_map<std::string, SuperObject*>::const_iterator it = _uniformMap.begin(); it != _uniformMap.end(); ++it)
	{
		it->second->update(shader);
	}
}

void Material::bindTextures()
{
	bindTextures(_shader);
}

void Material::bindTextures(Shader* shader)
{
	for (std::unordered_map<std::string, ImageBuffer*>::const_iterator it = _imageBufferMap.begin(); it != _imageBufferMap.end(); ++it)
	{
		if (shader->textures.find(it->first) != shader->textures.end())
			it->second->bind(shader->textures[it->first].unit);
	}
}

Shader* Material::getShader() 
{
	return _shader;
}