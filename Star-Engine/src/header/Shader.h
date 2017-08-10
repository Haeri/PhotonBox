#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>
#include "Display.h"
#include "Matrix4f.h"

class Shader {
public:
	GLint positionAttrib, normalAttrib, colorAttrib, uvAttrib;
	std::map<std::string, GLint> uniforms;

	Shader() {}
	Shader(const std::string& fileName);
	~Shader();

	virtual void addUniforms();
	void bind();
	void update(Matrix4f& matrix);
	void addUniform(std::string uniform);

private:
	const static unsigned int NUM_SHADERS = 2;

	GLuint _program;
	GLuint _shaders[NUM_SHADERS];

	std::string readShader(const std::string& fileName);
	GLuint createShader(const std::string& shaderSource, unsigned int shaderType);
	int checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
};

#endif /* defined(SHADER_H) */
