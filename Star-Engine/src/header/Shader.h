#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "Display.h"


class Shader {
public:
	void init(const std::string& fileName);
	void destroy();
	void bind();

private:
	const static unsigned int NUM_SHADERS = 2;

	GLuint _program;
	GLuint _shaders[NUM_SHADERS];

	std::string readShader(const std::string& fileName);
	GLuint createShader(const std::string& shaderSource, unsigned int shaderType);
	void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
};

#endif /* defined(SHADER_H) */
