#include "PhotonBox/resource/Shader.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

#include "PhotonBox/core/Resources.h"
#include "PhotonBox/core/system/ResourceManager.h"
#include "PhotonBox/resource/CubeMap.h"
#include "PhotonBox/resource/Texture.h"
#include "PhotonBox/util/Util.h"
#include "PhotonBox/util/FileWatch.h"
#include "PhotonBox/util/Logger.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

struct glsl_type_set {
	GLenum      type;
	const char* name;
}

type_set[] = {
  GL_INVALID_ENUM,                              "invalid",
  GL_FLOAT,                                     "float",
  GL_FLOAT_VEC2,                                "vec2",
  GL_FLOAT_VEC3,                                "vec3",
  GL_FLOAT_VEC4,                                "vec4",
  GL_DOUBLE,                                    "double",
  GL_DOUBLE_VEC2,                               "dvec2",
  GL_DOUBLE_VEC3,                               "dvec3",
  GL_DOUBLE_VEC4,                               "dvec4",
  GL_INT,                                       "int",
  GL_INT_VEC2,                                  "ivec2",
  GL_INT_VEC3,                                  "ivec3",
  GL_INT_VEC4,                                  "ivec4",
  GL_UNSIGNED_INT,                              "unsigned int",
  GL_UNSIGNED_INT_VEC2,                         "uvec2",
  GL_UNSIGNED_INT_VEC3,                         "uvec3",
  GL_UNSIGNED_INT_VEC4,                         "uvec4",
  GL_BOOL,                                      "bool",
  GL_BOOL_VEC2,                                 "bvec2",
  GL_BOOL_VEC3,                                 "bvec3",
  GL_BOOL_VEC4,                                 "bvec4",
  GL_FLOAT_MAT2,                                "mat2",
  GL_FLOAT_MAT3,                                "mat3",
  GL_FLOAT_MAT4,                                "mat4",
  GL_FLOAT_MAT2x3,                              "mat2x3",
  GL_FLOAT_MAT2x4,                              "mat2x4",
  GL_FLOAT_MAT3x2,                              "mat3x2",
  GL_FLOAT_MAT3x4,                              "mat3x4",
  GL_FLOAT_MAT4x2,                              "mat4x2",
  GL_FLOAT_MAT4x3,                              "mat4x3",
  GL_DOUBLE_MAT2,                               "dmat2",
  GL_DOUBLE_MAT3,                               "dmat3",
  GL_DOUBLE_MAT4,                               "dmat4",
  GL_DOUBLE_MAT2x3,                             "dmat2x3",
  GL_DOUBLE_MAT2x4,                             "dmat2x4",
  GL_DOUBLE_MAT3x2,                             "dmat3x2",
  GL_DOUBLE_MAT3x4,                             "dmat3x4",
  GL_DOUBLE_MAT4x2,                             "dmat4x2",
  GL_DOUBLE_MAT4x3,                             "dmat4x3",
  GL_SAMPLER_1D,                                "sampler1D",
  GL_SAMPLER_2D,                                "sampler2D",
  GL_SAMPLER_3D,                                "sampler3D",
  GL_SAMPLER_CUBE,                              "samplerCube",
  GL_SAMPLER_1D_SHADOW,                         "sampler1DShadow",
  GL_SAMPLER_2D_SHADOW,                         "sampler2DShadow",
  GL_SAMPLER_1D_ARRAY,                          "sampler1DArray",
  GL_SAMPLER_2D_ARRAY,                          "sampler2DArray",
  GL_SAMPLER_1D_ARRAY_SHADOW,                   "sampler1DArrayShadow",
  GL_SAMPLER_2D_ARRAY_SHADOW,                   "sampler2DArrayShadow",
  GL_SAMPLER_2D_MULTISAMPLE,                    "sampler2DMS",
  GL_SAMPLER_2D_MULTISAMPLE_ARRAY,              "sampler2DMSArray",
  GL_SAMPLER_CUBE_SHADOW,                       "samplerCubeShadow",
  GL_SAMPLER_BUFFER,                            "samplerBuffer",
  GL_SAMPLER_2D_RECT,                           "sampler2DRect",
  GL_SAMPLER_2D_RECT_SHADOW,                    "sampler2DRectShadow",
  GL_INT_SAMPLER_1D,                            "isampler1D",
  GL_INT_SAMPLER_2D,                            "isampler2D",
  GL_INT_SAMPLER_3D,                            "isampler3D",
  GL_INT_SAMPLER_CUBE,                          "isamplerCube",
  GL_INT_SAMPLER_1D_ARRAY,                      "isampler1DArray",
  GL_INT_SAMPLER_2D_ARRAY,                      "isampler2DArray",
  GL_INT_SAMPLER_2D_MULTISAMPLE,                "isampler2DMS",
  GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,          "isampler2DMSArray",
  GL_INT_SAMPLER_BUFFER,                        "isamplerBuffer",
  GL_INT_SAMPLER_2D_RECT,                       "isampler2DRect",
  GL_UNSIGNED_INT_SAMPLER_1D,                   "usampler1D",
  GL_UNSIGNED_INT_SAMPLER_2D,                   "usampler2D",
  GL_UNSIGNED_INT_SAMPLER_3D,                   "usampler3D",
  GL_UNSIGNED_INT_SAMPLER_CUBE,                 "usamplerCube",
  GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,             "usampler2DArray",
  GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,             "usampler2DArray",
  GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,       "usampler2DMS",
  GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, "usampler2DMSArray",
  GL_UNSIGNED_INT_SAMPLER_BUFFER,               "usamplerBuffer",
  GL_UNSIGNED_INT_SAMPLER_2D_RECT,              "usampler2DRect",
  GL_IMAGE_1D,                                  "image1D",
  GL_IMAGE_2D,                                  "image2D",
  GL_IMAGE_3D,                                  "image3D",
  GL_IMAGE_2D_RECT,                             "image2DRect",
  GL_IMAGE_CUBE,                                "imageCube",
  GL_IMAGE_BUFFER,                              "imageBuffer",
  GL_IMAGE_1D_ARRAY,                            "image1DArray",
  GL_IMAGE_2D_ARRAY,                            "image2DArray",
  GL_IMAGE_2D_MULTISAMPLE,                      "image2DMS",
  GL_IMAGE_2D_MULTISAMPLE_ARRAY,                "image2DMSArray",
  GL_INT_IMAGE_1D,                              "iimage1D",
  GL_INT_IMAGE_2D,                              "iimage2D",
  GL_INT_IMAGE_3D,                              "iimage3D",
  GL_INT_IMAGE_2D_RECT,                         "iimage2DRect",
  GL_INT_IMAGE_CUBE,                            "iimageCube",
  GL_INT_IMAGE_BUFFER,                          "iimageBuffer",
  GL_INT_IMAGE_1D_ARRAY,                        "iimage1DArray",
  GL_INT_IMAGE_2D_ARRAY,                        "iimage2DArray",
  GL_INT_IMAGE_2D_MULTISAMPLE,                  "iimage2DMS",
  GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY,            "iimage2DMSArray",
  GL_UNSIGNED_INT_IMAGE_1D,                     "uimage1D",
  GL_UNSIGNED_INT_IMAGE_2D,                     "uimage2D",
  GL_UNSIGNED_INT_IMAGE_3D,                     "uimage3D",
  GL_UNSIGNED_INT_IMAGE_2D_RECT,                "uimage2DRect",
  GL_UNSIGNED_INT_IMAGE_CUBE,                   "uimageCube",
  GL_UNSIGNED_INT_IMAGE_BUFFER,                 "uimageBuffer",
  GL_UNSIGNED_INT_IMAGE_1D_ARRAY,               "uimage1DArray",
  GL_UNSIGNED_INT_IMAGE_2D_ARRAY,               "uimage2DArray",
  GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,         "uimage2DMS",
  GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,   "uimage2DMSArray",
  GL_UNSIGNED_INT_ATOMIC_COUNTER,               "atomic_uint"
};

std::vector<Shader*> Shader::_shaderList;

void Shader::blankInitialize() 
{
	_program = glCreateProgram();
}

void Shader::init()
{
	_filePath = Filepath(getFilePath());
	
	FileWatch::addToWatchList(_filePath.getAbsolutePath(), this);

	Logger::logn("Index Shader: " + _filePath.getAbsolutePath());

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

bool Shader::loadFromFile()
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

	return !_fragmentCode.empty() && !_vertextCode.empty();
}

void Shader::submitBuffer()
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
	checkShaderError(_program, GL_LINK_STATUS, true, "\nSHADER-ERROR in '" + _filePath.getName() + "': Faild linking program!");

	glValidateProgram(_program);
	checkShaderError(_program, GL_VALIDATE_STATUS, true, "\nSHADER-ERROR in '" + _filePath.getName() + "': Shader Program invalid!");

	GLint numUniforms;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);
	for (int i = 0; i < numUniforms; i++) {
		int name_len = -1, num = -1;
		GLenum type = GL_ZERO;
		char name[100];
		glGetActiveUniform(_program, GLuint(i), sizeof(name) - 1, &name_len, &num, &type, name);
		name[name_len] = 0;
		GLuint location = glGetUniformLocation(_program, name);

		if (num > 1)
		{
			for (int i = 0; i < num; ++i)
			{
				std::string s_name(name);
				s_name.replace(s_name.find("[0]"), sizeof("[0]") - 1, "");
				s_name += "[" + std::to_string(i) + "]";
				
				location = glGetUniformLocation(_program, s_name.c_str());

				uniforms[s_name] = location;

				if (type >= GL_SAMPLER_1D && type <= GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY)
				{
					addTexture(s_name);
				}
				else
				{
					addUniform(s_name);
				}
			}
		}
		else
		{
			if (type >= GL_SAMPLER_1D && type <= GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY)
			{
				addTexture(name);
			}
			else
			{
				addUniform(name);
			}
		}
	}
}

void Shader::bind()
{
	if (!isInitialized()) return;

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
	if (!isInitialized()) return;

	for (std::map<std::string, GLint>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		glEnableVertexAttribArray(it->second);
	}
}

void Shader::disableAttributes()
{
	if (!isInitialized()) return;

	for (std::map<std::string, GLint>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		glDisableVertexAttribArray(it->second);
	}
}

void Shader::updateTextures()
{
	if (!isInitialized()) return;

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
		Logger::logn("Unable to open file " + fileName, Logger::ERR);
	}

	return text;
}

GLuint Shader::createShader(const std::string& shaderSource, unsigned int shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		Logger::logn("Failed creating shader type " + shaderType, Logger::ERR);
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

		Logger::logn(errorMessage + ":\n\t" + error, Logger::ERR);

		return 1;
	}
	else
	{
		return 0;
	}
}

bool Shader::checkUniform(const std::string & name)
{
	if (!isInitialized()) return false;
#ifdef _DEBUG
	if (uniforms.find(name) != uniforms.end())
	{
		return true;
	}
	else
	{
		Logger::logn("Uniform " + name + " does not exist in shader " + _filePath.getName(), Logger::WARN);
		return false;
	}
#else
	return uniforms.find(name) != uniforms.end();
#endif
}
