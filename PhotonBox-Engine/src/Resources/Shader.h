#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>
#include "../Core/Display.h"
#include "../Math/Matrix4f.h"



class Shader {
public:
	struct TexUniforUnit
	{
		GLuint uniformLocation;
		GLuint unit;
	};

	std::map<std::string, GLint> uniforms;
	std::map<std::string, GLint> attributes;
	std::map<std::string, TexUniforUnit> textures;

	void init(const std::string& fileName);
	void bind();
	void destroy();

	virtual void addUniforms() = 0;
	virtual void addAttributes() = 0;
	virtual void update(Matrix4f& matrix);

	void addAttribut(std::string attribute, GLint index);
	void addUniform(std::string uniform);
	void addTextureUnit(std::string uniform, GLuint unit);
	void enableAttributes();
	void disableAttributes();

private:
	const static unsigned int NUM_SHADERS = 2;

	GLuint _program;
	GLuint _shaders[NUM_SHADERS];

	std::string readShader(const std::string& fileName);
	GLuint createShader(const std::string& shaderSource, unsigned int shaderType);
	int checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
};

#endif /* defined(SHADER_H) */
