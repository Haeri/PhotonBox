#ifndef SHADER_H
#define SHADER_H

class Texture;
class CubeMap;
class Transform;
class LightEmitter;

#include <string>
#include <map>
#include "../Core/Display.h"
#include "../Math/Matrix4f.h"

template <class Instance>
class InstancedShader: public Shader {
public:
	static Instance* getInstance() {
		if (_instance == nullptr) {
			_instance = new Instance;
			_instance->init();
		}
		return _instance;
	}
protected:
	static Instance* _instance;
	InstancedShader<Instance>() {}
};

template <class Instance>
Instance* InstancedShader<Instance>::_instance = nullptr;

class Shader {
public:
	struct TexUniforUnit
	{
		GLint uniformLocation;
		GLint unit;
	};

	std::map<std::string, GLint> uniforms;
	std::map<std::string, GLint> attributes;
	std::map<std::string, TexUniforUnit> textures;

	void init();
	void bind();
	void destroy();

	virtual std::string getFilePath() = 0;
	virtual void addUniforms() = 0;
	virtual void addAttributes() = 0;
	virtual void update(Transform* transform) {}
	virtual void update(Transform* transform, LightEmitter* thing) {}

	void addAttribut(std::string attribute, GLint index);
	void addUniform(std::string uniform);
	void addTexture(std::string uniform);
	void enableAttributes();
	void disableAttributes();
	void updateTextures();
	
	std::string& getName() { return _fileName; }

	template<typename T>
	void setUniform(const std::string& uniformName, T value) {}
	template<>
	void setUniform<int>(const std::string& uniformName, int value) {
		glUniform1i(uniforms[uniformName], GLint(value));
	}
	template<>
	void setUniform<float>(const std::string& uniformName, float value) {
		glUniform1f(uniforms[uniformName], GLfloat(value));
	}
	template<>
	void setUniform<bool>(const std::string& uniformName, bool value){ 
		glUniform1i(uniforms[uniformName], GLint(value)); 
	}
	template<>
	void setUniform<Vector2f>(const std::string& uniformName, Vector2f value) {
		glUniform2fv(uniforms[uniformName], 1, &(value.x()));
	}
	template<>
	void setUniform<Vector3f>(const std::string& uniformName, Vector3f value) {
		glUniform3fv(uniforms[uniformName], 1, &(value.x()));
	}
	template<>
	void setUniform<Vector4f>(const std::string& uniformName, Vector4f value) {
		glUniform4fv(uniforms[uniformName], 1, &(value.x()));
	}
	template<>
	void setUniform<Matrix4f>(const std::string& uniformName, Matrix4f value) {
		glUniformMatrix4fv(uniforms[uniformName], 1, GL_FALSE, &(value(0, 0))); 
	}
	//void setUniform(const std::string& uniformName, Texture* texture);
	//void setUniform(const std::string& uniformName, CubeMap* cubeMap);
protected:
	const static unsigned int NUM_SHADERS = 2;
	std::string _fileName;
	GLuint _program;
	GLuint _shaders[NUM_SHADERS];
	GLenum _textureUnit = GL_TEXTURE0;

	std::string readShader(const std::string& fileName);
	GLuint createShader(const std::string& shaderSource, unsigned int shaderType);
	int checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);	
};

#endif /* defined(SHADER_H) */