#include "Shader.h"
#include <iostream>
#include <fstream>
#include "../Core/Util.h"
#include "Texture.h"
#include "CubeMap.h"

void Shader::init() {
	std::vector<std::string> path;

	std::string filePath = getFilePath();
	Util::split(filePath, "/", path);

	_fileName = path.back();
	std::cout << "Creating " << _fileName << std::endl;

	_program = glCreateProgram();
	_shaders[0] = createShader(readShader(filePath + ".vs"), GL_VERTEX_SHADER);
	_shaders[1] = createShader(readShader(filePath + ".fs"), GL_FRAGMENT_SHADER);

	glAttachShader(_program, _shaders[0]);
	glAttachShader(_program, _shaders[1]);

	addAttributes();

	glLinkProgram(_program);
	checkShaderError(_program, GL_LINK_STATUS, true, "ERROR: Faild linking program!");

	glValidateProgram(_program);
	checkShaderError(_program, GL_VALIDATE_STATUS, true, "ERROR: Shader Program invalid!");

	/*
	GLint numUniforms;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);
	for(int i = 0; i < numUniforms; i++) {
		int name_len = -1, num = -1;
		GLenum type = GL_ZERO;
		char name[100];
		glGetActiveUniform(_program, GLuint(i), sizeof(name) - 1, &name_len, &num, &type, name);
		name[name_len] = 0;
		GLuint location = glGetUniformLocation(_program, name);
		std::cout << name << " location:" << location << std::endl;
	}
	*/

	addUniforms();
}

void Shader::destroy() {
	glDetachShader(_program, _shaders[0]);
	glDetachShader(_program, _shaders[1]);
	glDeleteProgram(_program);
}

void Shader::bind() {
	glUseProgram(_program);
}

void Shader::addAttribut(std::string attribute, GLint index) {
	attributes[attribute] = index;
	glBindAttribLocation(_program, index, attribute.c_str());
}

void Shader::addUniform(std::string uniform) {
	GLint pos = glGetUniformLocation(_program, uniform.c_str());
	if (pos == -1) std::cout << "\t\tcould not find uniform '" << uniform << "' in shader " << _fileName << std::endl;
	uniforms[uniform] = pos;
}

void Shader::addTexture(std::string uniform) {
	TexUniforUnit texUnit;
	texUnit.uniformLocation = glGetUniformLocation(_program, uniform.c_str());
	if (texUnit.uniformLocation == -1) std::cout << "\t\tcould not find uniform '" << uniform << "' in shader " << _fileName << std::endl;
	texUnit.unit = _textureUnit++;
	textures[uniform] = texUnit;
}

void Shader::enableAttributes() {
	for (std::map<std::string, GLint>::const_iterator it = attributes.begin(); it != attributes.end(); ++it){
		glEnableVertexAttribArray(it->second);
	}
}

void Shader::disableAttributes() {
	for (std::map<std::string, GLint>::const_iterator it = attributes.begin(); it != attributes.end(); ++it){
		glDisableVertexAttribArray(it->second);
	}
}

void Shader::updateTextures(){
	for (std::map<std::string, TexUniforUnit>::const_iterator it = textures.begin(); it != textures.end(); ++it) {
		glUniform1i(it->second.uniformLocation, it->second.unit - GL_TEXTURE0);
	}
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
	lengths[0] = (GLuint) shaderSource.length();

	glShaderSource(shader, 1, shaderSourceArray, lengths);
	glCompileShader(shader);


	
	checkShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}

int Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
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
		
		return 1;
	}else {
		return 0;
	}
}

bool Shader::checkUniform(const std::string & name)
{
	return uniforms.find(name) != uniforms.end();
}
