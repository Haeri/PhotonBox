#ifndef MATERIAL_H
#define MATERIAL_H

#include <unordered_map>
#include "Shader.h"

using byte = unsigned char;

class Material {
public:
	Shader* shader;

	Material(Shader* shader): shader(shader) {}

	template<typename T>
	void setProperty(std::string name, T value) {
		//TODO: Fix memory leak
		_uniformMap[name] = new BaseObject<T>(value, name);
	}

	// Texture
	void setTexture(const std::string& uniformName, Texture* texture);
	// CubeMap
	void setCubeMap(const std::string& uniformName, CubeMap* cubeMap);
	
	

	void updateUniforms();
	void updateUniforms(Shader* shader);
	void bindTextures();
	void bindTextures(Shader* customShader);

	template <typename T>
	T getProperty(const std::string& uniformName) {
		return ((BaseObject<T>*)(_uniformMap[uniformName]))->_value;
	}
	Texture* getTexture(const std::string& uniformName);
	CubeMap* getCubeMap(const std::string& uniformName);
private:
	
	struct SuperObject {
		virtual void update(Shader* shader) = 0;
	};

	template<class T>
	struct BaseObject: SuperObject {
		std::string _name;
		T _value;
		BaseObject(T value, std::string name) : _value(value), _name(name) {}
		void update(Shader* shader) { shader->setUniform(_name, _value); }
	};

	std::unordered_map<std::string, Texture*> _textreMap;
	std::unordered_map<std::string, CubeMap*> _cubeMapMap;
	std::unordered_map<std::string, SuperObject*> _uniformMap;
};

#endif /* defined(MATERIAL_H) */