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

class Shader {
public:
	struct TexUniforUnit
	{
		GLint uniformLocation;
		GLenum unit;
	};

	std::map<std::string, GLint> uniforms;
	std::map<std::string, GLint> attributes;
	std::map<std::string, TexUniforUnit> textures;

	void init(const std::string& fileName);
	void bind();
	void destroy();

	virtual void addUniforms() = 0;
	virtual void addAttributes() = 0;
	virtual void update(Transform* transform) {}
	virtual void update(Transform* transform, LightEmitter* light) {}

	void addAttribut(std::string attribute, GLint index);
	void addUniform(std::string uniform);
	void addTexture(std::string uniform);
	void enableAttributes();
	void disableAttributes();
	void updateTextures();
	
	std::string& getName() { return _fileName; }


	void setUniform(const std::string& uniformName, int value);
	void setUniform(const std::string& uniformName, float value);
	void setUniform(const std::string& uniformName, bool value);
	void setUniform(const std::string& uniformName, Vector2f value);
	void setUniform(const std::string& uniformName, Vector3f value);
	void setUniform(const std::string& uniformName, Vector4f value);
	void setUniform(const std::string& uniformName, Matrix4f value);
	//void setUniform(const std::string& uniformName, Texture* texture);
	//void setUniform(const std::string& uniformName, CubeMap* cubeMap);
private:
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
