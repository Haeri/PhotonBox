#ifndef B_SHADER_DATA_GENERATOR_H
#define B_SHADER_DATA_GENERATOR_H

#include "IShaderData.h"
#include "MaterialData.h"
#include "Texture.h"

struct ShaderGeneratorSettings
{
	GLuint maxLights;				// The maximum number of light sources to be used 
	bool ambientLighting;			// Set true if the shader should support ambient lighting
	bool diffuseLighting;			// Set true if the shader should support diffuse lighting
	bool specularLighting;			// Set true if the shader should support specular lighting
	bool ambientColor;				// Set true if the material specifies an ambient color (usually Ka)
	bool diffuseColor;				// Set true if the material specifies a diffuse color (usually Kd)
	bool specularColor;				// Set true if the material specifies a specular color (usually Ks)
	bool diffuseMap;				// Set true if a texture should be used for diffuse coloring
	bool normalMap;					// Set true if a texture should be used to define the normal vectors
	bool specularMap;				// Set true if a texture should be used to define specularity
	bool transparencyValue;			// Set true if a transparency value should be passed
	bool variableNumberOfLights;	// Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	bool isText;					// Set true if the shader should be used for displaying text
};

/** @brief The underlying shader data is generated 
*	@author Benjamin Buergisser
*/
class ShaderDataGenerator : public IShaderData
{
public:

	/* Typedefs */
	typedef std::unordered_map<std::string, std::string>		TextureMap;
	typedef std::unordered_map<std::string, std::string>		CubeRelflectionMap;
	typedef std::unordered_map<std::string, vmml::Vector3f>		Vector3Map;
	typedef std::unordered_map<std::string, GLfloat>			ScalarMap;

	/* Functions */

	/**	@brief Constructor
	*/
	ShaderDataGenerator();

	/**	@brief Constructor
	*	@param[in] maxLights The maximum number of light sources to be used
	*	@param[in] ambientLighting Set true if the shader should support ambient lighting
	*	@param[in] materialData All necessary information for the shader is read from the material data
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*	@param[in] isText Set true if the shader should be used for displaying text
	*/
	ShaderDataGenerator(GLuint maxLights, bool ambientLighting, const MaterialData &materialData, bool variableNumberOfLights, bool isText);

	/**	@brief Constructor
	*	@param[in] shaderGeneratorSettings The settings defining the abilities the generated shader should have
	*/
	ShaderDataGenerator(const ShaderGeneratorSettings &shaderGeneratorSettings);

	/**	@brief Virtual destructor
	*/
	virtual ~ShaderDataGenerator() {}

	/**	@brief Constructor
	*	@param[in] shaderGeneratorSettings The settings defining the abilities the generated shader should have
	*/
	ShaderDataGenerator &create(const ShaderGeneratorSettings &shaderGeneratorSettings);

	/**	@brief Gets the source code of the vertex shader as a string
	*/
	std::string getVertShaderSrc()  const   { return _vertShaderSrc; }

	/**	@brief Gets the source code of the fragment shader as a string
	*/
	std::string getFragShaderSrc()  const   { return _fragShaderSrc; }

	/**	@brief Get the maximum number of lights
	*/
	GLuint getMaxLights() const { return _maxLights; }

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

	/**	@brief Returns true if the shader is valid
	*/
	bool        isValid()           const	{ return _valid; }

private:

	/* Functions */

	void buildShader();
	void initializeSourceCommonVariables();
	void createVertShader();
	void createFragShader();
	void readMaterialAttributes(GLuint maxLights, bool variableNumberOfLights, bool ambientLighting, bool isText, const TextureMap &t, const Vector3Map &v, const ScalarMap &s);
	
	/* Variables */

	std::string _vertShaderSrc;
	std::string _fragShaderSrc;
	bool        _valid;

	GLuint		_maxLights;
	bool		_variableNumberOfLights;
	bool		_ambientLighting;
	bool		_diffuseLighting;
	bool		_specularLighting;
	bool		_ambientColor;
	bool		_diffuseColor;
	bool		_specularColor;
	bool		_diffuseMap;
	bool		_normalMap;
	bool		_specularMap;
	bool		_cubicReflectionMap;
	bool		_transparencyValue;
	bool		_isText;

};

#endif /* defined(B_SHADER_DATA_GENERATOR_H) */
