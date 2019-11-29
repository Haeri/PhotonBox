#ifndef MATERIAL_H
#define MATERIAL_H

class CubeMap;
class Texture;
class ImageBuffer;

#include <unordered_map>

#include "PhotonBox/core/ManagedResource.h"
#include "PhotonBox/resource/Shader.h"

class Material : public ManagedResource
{
public:

	//Material() {}
	Material(std::string name, Shader* shader) : _shader(shader) {}
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

	void setImageBuffer(const std::string& uniformName, ImageBuffer* image);

	void updateUniforms();
	void updateUniforms(Shader* shader);
	void bindTextures();
	void bindTextures(Shader* customShader);
	Shader* getShader();

	template <typename T>
	T getProperty(const std::string& uniformName)
	{
		return ((BaseObject<T>*)(_uniformMap[uniformName]))->_value;
	}
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

	Shader* _shader;
	std::unordered_map<std::string, ImageBuffer*> _imageBufferMap;
	std::unordered_map<std::string, SuperObject*> _uniformMap;
};

#endif // MATERIAL_H
