#ifndef MATERIAL_H
#define MATERIAL_H

class CubeMap;
class Texture;

#include <unordered_map>

#include "PhotonBox/core/ManagedResource.h"
#include "PhotonBox/resource/FrameBuffer.h"
#include "PhotonBox/resource/Shader.h"

class Material : public ManagedResource
{
public:
	Shader * shader;

	//TODO: Assign default PBR shader if no shader provided
	Material() {}
	Material(Shader* shader) : shader(shader) {}
	~Material();

	template<typename T>
	void setProperty(std::string name, T value)
	{
		if (_uniformMap.find(name) != _uniformMap.end())
		{
			((BaseObject<T>*)(_uniformMap[name]))->_value = value;
		}
		else
		{
			_uniformMap[name] = new BaseObject<T>(value, name);
		}
	}

	// Texture
	void setTexture(const std::string& uniformName, Texture* texture);
	void setTexture(const std::string & uniformName, FrameBuffer * buffer, std::string attachmentName);
	void setCubeMap(const std::string& uniformName, CubeMap* cubeMap);

	void updateUniforms();
	void updateUniforms(Shader* shader);
	void bindTextures();
	void bindTextures(Shader* customShader);

	template <typename T>
	T getProperty(const std::string& uniformName)
	{
		return ((BaseObject<T>*)(_uniformMap[uniformName]))->_value;
	}
	Texture* getTexture(const std::string& uniformName);
	CubeMap* getCubeMap(const std::string& uniformName);
private:
	struct SuperObject
	{
		virtual void update(Shader* shader) = 0;
		virtual ~SuperObject() {}
	};

	template<class T>
	struct BaseObject : SuperObject
	{
		std::string _name;
		T _value;
		BaseObject(T value, std::string name) : _value(value), _name(name) {}
		void update(Shader* shader) { shader->setUniform(_name, _value); }
	};

	std::unordered_map<std::string, Texture*> _textreMap;
	std::unordered_map<std::string, FrameBuffer::BufferAttachment*> _frameBufferMap;
	std::unordered_map<std::string, CubeMap*> _cubeMapMap;
	std::unordered_map<std::string, SuperObject*> _uniformMap;
};

#endif // MATERIAL_H
