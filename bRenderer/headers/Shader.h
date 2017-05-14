#ifndef B_SHADER_H
#define B_SHADER_H

#include <memory>
#include <string>
#include <unordered_map>
#include "vmmlib/matrix.hpp"
#include "Renderer_GL.h"
#include "Texture.h"


class IShaderData;

/** @brief Shader 
*	@author Rahul Mukhi, David Steiner
*/
class Shader
{
public:

	/* Structs */

    struct Attrib
    {
        GLint   loc;
        GLint   size;
        GLenum  type;
        GLsizei stride;     // size of vertex data structure
        size_t  offset;
    };

	/* Typedefs */

    typedef std::unordered_map< std::string, GLint >    LocationMap;
    typedef std::unordered_map< std::string, Attrib >   AttribMap;
    
	/* Functions */

	/**	@brief Constructor
	*	@param[in] shaderData
	*/
	Shader(const IShaderData &shaderData);

	/**	@brief Virtual destructor
	*/
	virtual ~Shader()
	{
		deleteShader();
	}

	/**	@brief Binds the shader and its attributes
	*/
    virtual void bind();
    
	/**	@brief Pass a 4 dimensional matrix to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg 4 dimensional matrix
	*/
    virtual void setUniform(const std::string &name, const vmml::Matrix4f &arg);

	/**	@brief Pass a 3 dimensional matrix to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg 3 dimensional matrix
	*/
    virtual void setUniform(const std::string &name, const vmml::Matrix3f &arg);

	/**	@brief Pass a 4 dimensional vector to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg 4 dimensional vector
	*/
    virtual void setUniform(const std::string &name, const vmml::Vector4f &arg);

	/**	@brief Pass a 3 dimensional vector to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg 3 dimensional vector
	*/
    virtual void setUniform(const std::string &name, const vmml::Vector3f &arg);

	/**	@brief Pass a floating-point number to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg Floating-point number
	*/
    virtual void setUniform(const std::string &name, GLfloat arg);
    
	/**	@brief Pass a texture to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg Texture
	*/
    virtual void setUniform(const std::string &name, TexturePtr texture);
    
	/**	@brief Register a uniform
	*	@param[in] name Uniform name
	*/
    virtual GLint registerUniform(const std::string &name);

	/**	@brief Register an attribute
	*	@param[in] name Attribute name
	*	@param[in] size
	*	@param[in] type
	*	@param[in] stride
	*	@param[in] offset
	*/
    virtual GLint registerAttrib(const std::string &name, GLint size, GLenum type, GLsizei stride, size_t offset);
    
	/**	@brief Returns the shader ID
	*/
	GLuint getProgramID() { return _programID; }
    
	/**	@brief Returns uniform location (tries to register uniform if not already available)
	*	@param[in] name Uniform name
	*/
    virtual GLint findUniformLocation(const std::string &name);

	/**	@brief Returns attribute location (tries to register attribute if not already available)
	*	@param[in] name Attribute name
	*	@param[in] size
	*	@param[in] type
	*	@param[in] stride
	*	@param[in] offset
	*/
    virtual GLint findAttribLocation(const std::string &name, GLint size, GLenum type, GLsizei stride, size_t offset);

	/**	@brief Returns uniform location
	*	@param[in] name Uniform name
	*/
    virtual GLint getUniformLocation(const std::string &name);

	/**	@brief Returns attribute location
	*	@param[in] name Attribute name
	*/
    virtual GLint getAttribLocation(const std::string &name);

	/**	@brief Get the maximum number of lights used 
	*/
	GLuint getMaxLights() { return _shaderMaxLights; }

	/**	@brief Returns true if the number of lights is variable in the shader
	*/
	bool supportsVariableNumberOfLights() const { return _variableNumberOfLights; }

	/**	@brief Returns true if the shader supports ambient lighting
	*/
	bool supportsAmbientLighting() const	{ return _ambientLighting; }

	/**	@brief Returns true if the shader supports diffuse lighting
	*/
	bool supportsDiffuseLighting() const	{ return _diffuseLighting; }

	/**	@brief Returns true if the shader supports specular lighting
	*/
	bool supportsSpecularLighting() const	{ return _specularLighting; }

	/**	@brief Returns true if the shader supports a cubic reflection map
	*/
	bool supportsCubicReflectionMap() const { return _cubicReflectionMap; }
    
    template< typename T >
	/**	@brief Sets multiple uniforms
	*	@param[in] arg Data to pass to the shader
	*/
    void setUniforms(const T &arg)
    {
        for (auto i = arg.cbegin(); i != arg.cend(); ++i)
        {
            setUniform(i->first, i->second);
        }
    }

	/**	@brief Returns the current texture unit used in the shader
	*/
	GLint getCurrentTexUnit() { return GL_TEXTURE0 + _cTexUnit; }
    
	/**	@brief Deletes the shader
	*/
	virtual void deleteShader()
	{
		if (_programID) {
			glDeleteProgram(_programID);
		}
	}

protected:
	
	/* Functions */

    virtual void resetTexUnit();
	virtual bool compile(GLuint* shader, GLenum type, const std::string &src);
	virtual bool link();
	virtual bool validate();
    
private:

	/* Variables */

    GLuint _programID = 0;
    
    GLint   _cTexUnit = 0;
    GLint   _maxTexUnits = 0;
    
	LocationMap   _uniformLocations;
	AttribMap     _attribs;

	GLuint		_shaderMaxLights;
	bool		_variableNumberOfLights;
	bool		_ambientLighting;
	bool		_diffuseLighting;
	bool		_specularLighting;
	bool		_cubicReflectionMap;
};

typedef std::shared_ptr< Shader > ShaderPtr;

#endif /* defined(B_SHADER_H) */
