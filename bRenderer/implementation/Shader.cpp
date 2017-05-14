#include "headers/Logger.h"
#include "headers/GeometryData.h"
#include "headers/IShaderData.h"
#include "headers/Shader.h"

Shader::Shader(const IShaderData &shaderData)
{
    GLuint vertShader, fragShader;
    
    // Create shader program.
    _programID = glCreateProgram();
    
    if(!compile(&vertShader, GL_VERTEX_SHADER, shaderData.getVertShaderSrc())) {
        bRenderer::log("Failed to compile vertex shader");
    }
    
    if (!compile(&fragShader, GL_FRAGMENT_SHADER, shaderData.getFragShaderSrc())) {
        bRenderer::log("Failed to compile fragment shader");
    }

	_shaderMaxLights = shaderData.getMaxLights();
	_variableNumberOfLights = shaderData.supportsVariableNumberOfLights();
	_ambientLighting = shaderData.supportsAmbientLighting();
	_diffuseLighting = shaderData.supportsDiffuseLighting();
	_specularLighting = shaderData.supportsSpecularLighting();
	_cubicReflectionMap = shaderData.supportsCubicReflectionMap();
    
    // Attach vertex shader to program.
    glAttachShader(_programID, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(_programID, fragShader);
        
    // Link program.
    if (!link()) {
		bRenderer::log("Failed to link program: " + std::to_string(_programID));
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (_programID) {
            glDeleteProgram(_programID);
            _programID = 0;
        }
    }
    
    resetTexUnit();
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTexUnits);
	bRenderer::log("Number of available tex units: " + std::to_string(_maxTexUnits)+".", bRenderer::LM_INFO);
}

void Shader::setUniform(const std::string &name, const vmml::Vector4f &arg)
{
    glUseProgram(_programID);
    
    GLint loc = findUniformLocation(name);
    if (loc > -1)
    {
        glUniform4fv(loc, 1, arg.begin());
    }
}

void Shader::setUniform(const std::string &name, const vmml::Vector3f &arg)
{
    glUseProgram(_programID);
    
    GLint loc = findUniformLocation(name);
    if (loc > -1)
    {
        glUniform3fv(loc, 1, arg.begin());
    }
}

void Shader::setUniform(const std::string &name, const vmml::Matrix4f &arg)
{
    glUseProgram(_programID);
    
    GLint loc = findUniformLocation(name);
    if (loc > -1)
    {
        glUniformMatrix4fv(loc, 1, false, arg.begin());
    }
}

void Shader::setUniform(const std::string &name, const vmml::Matrix3f &arg)
{
    glUseProgram(_programID);
    
    GLint loc = findUniformLocation(name);
    if (loc > -1)
    {
        glUniformMatrix3fv(loc, 1, false, arg.begin());
    }
}

void Shader::setUniform(const std::string &name, GLfloat arg)
{
    glUseProgram(_programID);
    
    GLint loc = findUniformLocation(name);
    if (loc > -1)
    {
        glUniform1f(loc, arg);
    }
}

void Shader::setUniform(const std::string &name, TexturePtr texture)
{
    glUseProgram(_programID);

    GLint loc = findUniformLocation(name);
    if (loc > -1)
    {
        texture->bind(GL_TEXTURE0 + _cTexUnit);
        glUniform1i(loc, _cTexUnit);
        
        _cTexUnit = (_cTexUnit + 1) % _maxTexUnits;
    }
}

GLint Shader::registerUniform(const std::string &name)
{
    GLint loc = glGetUniformLocation(_programID, name.c_str());
    
    // add 1 because -1 is returned by glGetUniformLocation if operation fails,
    // but the map's default value is 0.
    _uniformLocations[name] = loc + 1;
    
    return loc;
}

GLint Shader::registerAttrib(const std::string &name, GLint size, GLenum type, GLsizei stride, size_t offset)
{
    GLint loc = glGetAttribLocation(_programID, name.c_str());
    Attrib &attrib = _attribs[name];
    
    // add 1 because -1 is returned by glGetUniformLocation if operation fails,
    // but the map's default value is 0.
    attrib.loc = loc + 1;
    attrib.size = size;
    attrib.type = type;
    attrib.stride = stride;
    attrib.offset = offset;
    
    glEnableVertexAttribArray(loc);
    return loc;
}

GLint Shader::findUniformLocation(const std::string &name)
{
    GLint loc = getUniformLocation(name);
    if (loc < 0)
    {
        loc = registerUniform(name);
    }
    if (loc < 0)
    {
		bRenderer::log("Couldn't find uniform '" + name + "'.", bRenderer::LM_WARNING);
    }
    return loc;
}

GLint Shader::findAttribLocation(const std::string &name, GLint size, GLenum type, GLsizei stride, size_t offset)
{
    GLint loc = getAttribLocation(name);
    if (loc < 0)
    {
        loc = registerAttrib(name, size, type, stride, offset);
    }
    if (loc < 0)
    {
		bRenderer::log("Couldn't find attrib '" + name + "'.", bRenderer::LM_WARNING);
    }
    return loc;
}

GLint Shader::getUniformLocation(const std::string &name)
{
    return _uniformLocations[name] - 1;     // subtract 1 again
}

GLint Shader::getAttribLocation(const std::string &name)
{
    return _attribs[name].loc - 1;     // subtract 1 again
}

void Shader::resetTexUnit()
{
    _cTexUnit = 0;
}

void Shader::bind()
{
    glUseProgram(_programID);
    
    for (auto i = _attribs.begin(); i != _attribs.end(); ++i)
    {
        const Attrib &attrib = i->second;
        GLint loc = attrib.loc - 1;
        if (loc > -1)
        {
            glVertexAttribPointer(loc, attrib.size, attrib.type, GL_FALSE, attrib.stride, reinterpret_cast<void*>(attrib.offset));
        }
        else
        {
//			bRenderer::log("Couldn't bind attrib '" + i->first + "' because its location is not valid.", bRenderer::LM_ERROR);
        }
    }
    
    resetTexUnit();
}

bool Shader::compile(GLuint* shader, GLenum type, const std::string &src)
{
    GLint status;
    
    if (src.length() < 1) {
		bRenderer::log("Failed to load vertex shader", bRenderer::LM_ERROR);
        return false;
    }

    *shader = glCreateShader(type);

    const GLchar *source = src.c_str();
    glShaderSource(*shader, 1, &source, NULL);

    glCompileShader(*shader);

    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::string log(logLength, ' ');
        glGetShaderInfoLog(*shader, logLength, &logLength, &(*log.begin()));
		bRenderer::log("Shader compile log:\n" + log);
    }
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return false;
    }
    
    return true;
}

bool Shader::link()
{
    GLint status;
    glLinkProgram(_programID);
    
    GLint logLength;
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::string log(logLength, ' ');
        glGetProgramInfoLog(_programID, logLength, &logLength, &(*log.begin()));
        bRenderer::log("Program link log:\n" + log);
    }
    
    glGetProgramiv(_programID, GL_LINK_STATUS, &status);
    if (status == 0) {
        return false;
    }
    
    return true;
}

bool Shader::validate()
{
    GLint logLength, status;
    
    glValidateProgram(_programID);
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::string log(logLength, ' ');
        glGetProgramInfoLog(_programID, logLength, &logLength, &(*log.begin()));
		bRenderer::log("Program validate log:\n" + log);
    }
    
    glGetProgramiv(_programID, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return true;
    }
    
    return false;
}
