#include "../header/Shader.h"
#include <iostream>
#include <fstream>


void Shader::init(const std::string& fileName) {
	_program = glCreateProgram();
	_shaders[0] = createShader(readShader(fileName + ".vs"), GL_VERTEX_SHADER);
	_shaders[0] = createShader(readShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	glAttachShader(_program, _shaders[0]);
	glAttachShader(_program, _shaders[1]);

	glLinkProgram(_program);
	checkShaderError(_program, GL_LINK_STATUS, true, "ERROR: Faild linking program!");

	glValidateProgram(_program);
	checkShaderError(_program, GL_VALIDATE_STATUS, true, "ERROR: Shader Program invalid!");
}

void Shader::destroy() {
	glDetachShader(_program, _shaders[0]);
	glDetachShader(_program, _shaders[1]);
	glDeleteProgram(_program);
}

void Shader::bind() {
	glUseProgram(_program);
}

std::string Shader::readShader(const std::string& fileName) {
		std::string line, text;
		std::ifstream myfile(fileName);

		if (myfile.is_open()){
			while (getline(myfile, line)){
				text += line + "\n";
			}
			myfile.close();
		}else {
			std::cerr << "Unable to open file " << fileName << std::endl;
		}

		return text;
}

GLuint Shader::createShader(const std::string& shaderSource, unsigned int shaderType) {
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0){
		std::cerr << "ERROR: Failed creating shader type " << shaderType << std::endl;
		return 0;
	}

	const GLchar* shaderSourceArray[1];
	shaderSourceArray[0] = shaderSource.c_str();
	GLint lengths[1];
	lengths[0] = shaderSource.length();

	glShaderSource(shader, 1, shaderSourceArray, lengths);
	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}