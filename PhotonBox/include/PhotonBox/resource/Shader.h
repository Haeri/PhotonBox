#ifndef SHADER_H
#define SHADER_H

class Texture;
class CubeMap;
class Transform;
class LightEmitter;

#include <string>
#include <map>
#include <vector>

#include "PhotonBox/core/ManagedResource.h"
#include "PhotonBox/core/ILazyLoadable.h"
#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/math/Matrix4f.h"
#include "PhotonBox/data-type/Vertex.h"

class Shader : public ManagedResource, public ILazyLoadable
{
public:

	enum Type {
		SURFACE_SHADER,
		SCREEN_SHADER,
		COMPUTE_SHADER,
		CUSTOM_SHADER,
	};

	struct TexUniforUnit
	{
		GLint uniformLocation;
		GLint unit;
	};

	Shader()
	{
		_shaderList.push_back(this);
	}

	// TODO: these attribute should be private. Check the query to 
	// prevent unwanted creation of new objects while querying
	std::map<std::string, GLint> uniforms;
	std::map<std::string, GLint> attributes;
	std::map<std::string, TexUniforUnit> textures;

	void init();
	void bind();
	void destroy();
	void loadFromFile() override;
	void sendToGPU() override;

	virtual std::string getFilePath() = 0;
	virtual void addUniforms() = 0;
	virtual void addAttributes() = 0;
	virtual void update(Transform* transform) {}
	virtual void update(Transform* transform, LightEmitter* thing) {}
	virtual Type getType() { return Type::CUSTOM_SHADER; }
	//virtual void update(void* data) {}

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
	void setUniform<int>(const std::string& uniformName, int value)
	{
		if (checkUniform(uniformName))
			glUniform1i(uniforms[uniformName], GLint(value));
	}
	template<>
	void setUniform<float>(const std::string& uniformName, float value)
	{
		if (checkUniform(uniformName))
			glUniform1f(uniforms[uniformName], GLfloat(value));
	}
	template<>
	void setUniform<bool>(const std::string& uniformName, bool value)
	{
		if (checkUniform(uniformName))
			glUniform1i(uniforms[uniformName], GLint(value));
	}
	template<>
	void setUniform<Vector2f>(const std::string& uniformName, Vector2f value)
	{
		if (checkUniform(uniformName))
			glUniform2fv(uniforms[uniformName], 1, &(value.x()));
	}
	template<>
	void setUniform<Vector3f>(const std::string& uniformName, Vector3f value)
	{
		if (checkUniform(uniformName))
			glUniform3fv(uniforms[uniformName], 1, &(value.x()));
	}
	template<>
	void setUniform<Vector4f>(const std::string& uniformName, Vector4f value)
	{
		if (checkUniform(uniformName))
			glUniform4fv(uniforms[uniformName], 1, &(value.x()));
	}
	template<>
	void setUniform<Matrix4f>(const std::string& uniformName, Matrix4f value)
	{
		if (checkUniform(uniformName))
			glUniformMatrix4fv(uniforms[uniformName], 1, GL_FALSE, &(value(0, 0)));
	}
	//void setUniform(const std::string& uniformName, Texture* texture);
	//void setUniform(const std::string& uniformName, CubeMap* cubeMap);

	static void clearAll();
	
protected:

	const static unsigned int NUM_SHADERS = 2;
	std::string _fileName;
	GLuint _program = -1;
	GLuint _shaders[NUM_SHADERS];
	GLenum _textureUnit = 0;
	static std::vector<Shader*> _shaderList;

	std::string readShader(const std::string& fileName);
	GLuint createShader(const std::string& shaderSource, unsigned int shaderType);
	int checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

private:
	std::string _vertextCode;
	std::string _fragmentCode;

	bool checkUniform(const std::string& name);

	void blankInitialize() override;

};


template <class Instance>
class InstancedShader : public Shader
{
public:
	static Instance* getInstance()
	{
		if (_instance == nullptr)
		{
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

#endif // SHADER_H
