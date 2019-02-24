#include "PhotonBox/resources/Shader.h"

#include <iostream>
#include <fstream>
#include <intrin.h>

#include "PhotonBox/core/Util.h"
#include "PhotonBox/resources/CubeMap.h"
#include "PhotonBox/resources/Texture.h"
#include "PhotonBox/core/systems/ResourceManager.h"
#include "PhotonBox/resources/Resources.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

std::vector<Shader*> Shader::_shaderList;

void Shader::blankInitialize() 
{
	_program = glCreateProgram();
}

void Shader::init()
{
	FileWatch::addToWatchList(getFilePath(), this);

	std::cerr << "Index Shader: " << getFilePath() << std::endl;

	std::vector<std::string> path;
	std::string filePath = getFilePath();
	Util::split(filePath, "/", path);

	_fileName = path.back();


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

	loadAsync();
}

void Shader::destroy()
{
	glDetachShader(_program, _shaders[0]);
	glDetachShader(_program, _shaders[1]);
	glDeleteShader(_shaders[0]);
	glDeleteShader(_shaders[1]);
	glDeleteProgram(_program);
}

void Shader::loadFromFile()
{
	if (this->getType() == Type::SCREEN_SHADER)
	{
		// TODO: Refactor Post shaders to use one default vertex shader
		//_vertextCode = readShader(Resources::ENGINE_RESOURCES + "/shader/util/default-post.vs");
		_vertextCode = readShader(getFilePath() + ".vs");
	}else{
		_vertextCode = readShader(getFilePath() + ".vs");
	}
	_fragmentCode = readShader(getFilePath() + ".fs");
	_isLoaded = true;
}

void Shader::sendToGPU()
{
	if (_program != -1)
	{
		destroy();

		uniforms.clear();
		attributes.clear();
		textures.clear();

		_program = glCreateProgram();
	}

	_shaders[0] = createShader(_vertextCode, GL_VERTEX_SHADER);
	_shaders[1] = createShader(_fragmentCode, GL_FRAGMENT_SHADER);

	glAttachShader(_program, _shaders[0]);
	glAttachShader(_program, _shaders[1]);

	addAttributes();

	glLinkProgram(_program);
	checkShaderError(_program, GL_LINK_STATUS, true, "\nSHADER-ERROR in '" + _fileName + "': Faild linking program!");

	glValidateProgram(_program);
	checkShaderError(_program, GL_VALIDATE_STATUS, true, "\nSHADER-ERROR in '" + _fileName + "': Shader Program invalid!");

	addUniforms();

	std::cout << "Initialized: " << _fileName << "\n";
}

void Shader::bind()
{
	glUseProgram(_program);
}

void Shader::addAttribut(std::string attribute, GLint index)
{
	attributes[attribute] = index;
	glBindAttribLocation(_program, index, attribute.c_str());
}

void Shader::addUniform(std::string uniform)
{
	GLint pos = glGetUniformLocation(_program, uniform.c_str());
	if (pos == -1) std::cout << "\t\tcould not find uniform '" << uniform << std::endl;
	uniforms[uniform] = pos;
}

void Shader::addTexture(std::string uniform)
{
	TexUniforUnit texUnit;
	texUnit.uniformLocation = glGetUniformLocation(_program, uniform.c_str());
	if (texUnit.uniformLocation == -1) std::cout << "\t\tcould not find uniform '" << uniform << std::endl;
	texUnit.unit = _textureUnit++;
	textures[uniform] = texUnit;
}

void Shader::enableAttributes()
{
	for (std::map<std::string, GLint>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		glEnableVertexAttribArray(it->second);
	}
}

void Shader::disableAttributes()
{
	for (std::map<std::string, GLint>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		glDisableVertexAttribArray(it->second);
	}
}

void Shader::updateTextures()
{
	for (std::map<std::string, TexUniforUnit>::const_iterator it = textures.begin(); it != textures.end(); ++it)
	{
		glUniform1i(it->second.uniformLocation, it->second.unit);
	}
}

void Shader::clearAll()
{
	for (std::vector<Shader*>::iterator it = _shaderList.begin(); it != _shaderList.end(); ++it)
	{
		delete (*it);
	}
	_shaderList.clear();
}

std::string Shader::readShader(const std::string& fileName)
{
	std::string line, text;
	std::ifstream myfile(fileName);

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			text += line + "\n";
		}
		myfile.close();
	}
	else
	{
		std::cerr << "Unable to open file " << fileName << std::endl;
	}

	return text;
}

GLuint Shader::createShader(const std::string& shaderSource, unsigned int shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		std::cerr << "ERROR: Failed creating shader type " << shaderType << std::endl;
		return 0;
	}

	const GLchar* shaderSourceArray[1];
	shaderSourceArray[0] = shaderSource.c_str();
	GLint lengths[1];
	lengths[0] = (GLuint)shaderSource.length();

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

		std::cerr << errorMessage << ":\n\t" << error;

		return 1;
	}
	else
	{
		return 0;
	}
}

bool Shader::checkUniform(const std::string & name)
{
#ifdef _DEBUG
	if (uniforms.find(name) != uniforms.end())
	{
		return true;
	}
	else
	{
		std::cout << "Uniform " << name << " does not exist in shader " << _fileName << std::endl;
		//__debugbreak();
		return false;
	}
#else
	return uniforms.find(name) != uniforms.end();
#endif
}
