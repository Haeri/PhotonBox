#ifndef B_I_SHADER_DATA_H
#define B_I_SHADER_DATA_H

#include <string>
#include "Renderer_GL.h"

/** @brief Interface for the underlying data of a shader
*	@author Benjamin Buergisser
*/
class IShaderData
{
public:

	/* Functions */

	/**	@brief Virtual destructor
	*/
	virtual ~IShaderData(){}

	/**	@brief Gets the source code of the vertex shader as a string
	*/
	virtual std::string getVertShaderSrc() const  = 0;

	/**	@brief Gets the source code of the fragment shader as a string
	*/
	virtual std::string getFragShaderSrc() const = 0;

	/**	@brief Get the maximum number of lights
	*/
	virtual GLuint getMaxLights() const = 0;

	/**	@brief Returns true if the number of lights is variable in the shader
	*/
	virtual bool supportsVariableNumberOfLights() const = 0;

	/**	@brief Returns true if the shader supports ambient lighting
	*/
	virtual bool supportsAmbientLighting() const = 0;

	/**	@brief Returns true if the shader supports diffuse lighting
	*/
	virtual bool supportsDiffuseLighting() const = 0;

	/**	@brief Returns true if the shader supports specular lighting
	*/
	virtual bool supportsSpecularLighting() const = 0;

	/**	@brief Returns true if the shader supports a cubic reflection map
	*/
	virtual bool supportsCubicReflectionMap() const = 0;

	/**	@brief Returns true if the shader is valid
	*/
	virtual bool        isValid() const = 0;

};

#endif /* defined(B_I_SHADER_DATA_H) */
