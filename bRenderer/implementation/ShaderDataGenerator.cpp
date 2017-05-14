#include "headers/ShaderDataGenerator.h"
#include "headers/Logger.h"
#include "headers/OSdetect.h"
#include "headers/Configuration.h"
#include "headers/ShaderSource.h"

/* Public functions */

ShaderDataGenerator::ShaderDataGenerator()
	: _valid(false)
{}

ShaderDataGenerator::ShaderDataGenerator(GLuint maxLights, bool ambientLighting, const MaterialData &materialData, bool variableNumberOfLights, bool isText)
	: _valid(false)
{
	// create shader fitting the needs of the material
	readMaterialAttributes(maxLights, variableNumberOfLights, ambientLighting, isText, materialData.textures, materialData.vectors, materialData.scalars);
	buildShader();
}

ShaderDataGenerator::ShaderDataGenerator(const ShaderGeneratorSettings &shaderGeneratorSettings)
	: _valid(false)
{
	create(shaderGeneratorSettings);
}

ShaderDataGenerator &ShaderDataGenerator::create(const ShaderGeneratorSettings &shaderGeneratorSettings)
{
	_maxLights = shaderGeneratorSettings.maxLights;
	_variableNumberOfLights = shaderGeneratorSettings.variableNumberOfLights;

	_ambientLighting = shaderGeneratorSettings.ambientLighting;
	_diffuseLighting = shaderGeneratorSettings.diffuseLighting;
	_specularLighting = shaderGeneratorSettings.specularLighting;

	_ambientColor = shaderGeneratorSettings.ambientColor;
	_diffuseColor = shaderGeneratorSettings.diffuseColor;
	_specularColor = shaderGeneratorSettings.specularColor;

	_diffuseMap = shaderGeneratorSettings.diffuseMap;
	_normalMap = shaderGeneratorSettings.normalMap;
	_specularMap = shaderGeneratorSettings.specularMap;
	_cubicReflectionMap = false;

	_transparencyValue = shaderGeneratorSettings.transparencyValue;

	_isText = shaderGeneratorSettings.isText;

	buildShader();
	
	return *this;
}

/* Private functions */

void ShaderDataGenerator::readMaterialAttributes(GLuint maxLights, bool variableNumberOfLights, bool ambientLighting, bool isText, const TextureMap &t, const Vector3Map &v, const ScalarMap &s)
{
	_maxLights = maxLights;
	_variableNumberOfLights = variableNumberOfLights;

	_ambientLighting = ambientLighting;

	_specularLighting = _maxLights > 0 && s.count(bRenderer::WAVEFRONT_MATERIAL_SPECULAR_EXPONENT()) > 0;

	_ambientColor = v.count(bRenderer::WAVEFRONT_MATERIAL_AMBIENT_COLOR()) > 0;
	_diffuseColor = v.count(bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_COLOR()) > 0;
	_specularColor = _specularLighting && v.count(bRenderer::WAVEFRONT_MATERIAL_SPECULAR_COLOR()) > 0;
	_diffuseMap = t.count(bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP()) > 0;
	_normalMap = _maxLights > 0 && t.count(bRenderer::DEFAULT_SHADER_UNIFORM_NORMAL_MAP()) > 0;
	_specularMap = _specularLighting && t.count(bRenderer::DEFAULT_SHADER_UNIFORM_SPECULAR_MAP()) > 0;

	_cubicReflectionMap = false;

	_diffuseLighting = _diffuseMap || _diffuseColor;

	_transparencyValue = s.count(bRenderer::DEFAULT_SHADER_UNIFORM_TRANSPARENCY()) > 0;

	_isText = isText;
}

void ShaderDataGenerator::buildShader()
{
	initializeSourceCommonVariables();
	createVertShader();
	createFragShader();

	bRenderer::log("Generated shader code: \n");
	bRenderer::log(_vertShaderSrc);
	bRenderer::log(_fragShaderSrc);

	_valid = true;
}

void ShaderDataGenerator::initializeSourceCommonVariables()
{
	std::string common = "";

#ifdef B_OS_DESKTOP	
	common = bRenderer::SHADER_SOURCE_HEAD_DESKTOP();
#endif
#ifdef B_OS_IOS
	common = bRenderer::SHADER_SOURCE_HEAD_ES();
#endif	
	// lights
	if (_variableNumberOfLights)
		common += bRenderer::SHADER_SOURCE_NUM_LIGHTS();
	common += bRenderer::shader_source_light_properties(_maxLights, _normalMap, _diffuseLighting, _specularLighting);
	// varyings
	if (_diffuseMap || _normalMap || _specularMap || _isText)
		common += bRenderer::SHADER_SOURCE_VARYINGS_TEX_COORD();
	if (!_normalMap && (_diffuseLighting || _specularLighting))
		common += bRenderer::SHADER_SOURCE_VARYINGS_NORMAL();
	if (_normalMap && _specularLighting)
		common += bRenderer::SHADER_SOURCE_VARYINGS_CAMERA_TANGENT();
	else if (_specularLighting)
		common += bRenderer::SHADER_SOURCE_VARYINGS_CAMERA_VIEW();

	_vertShaderSrc = common;
	_fragShaderSrc = common;
}

void ShaderDataGenerator::createVertShader()
{
	// matrices
	_vertShaderSrc += bRenderer::SHADER_SOURCE_MATRICES();
	// attributes
	_vertShaderSrc += bRenderer::SHADER_SOURCE_ATTRIBUTES();
	
	// main function begin
	_vertShaderSrc += bRenderer::shader_source_function_vertex_main_begin((_diffuseLighting || _specularLighting), (_diffuseMap || _normalMap || _specularMap || _isText), _normalMap);
	if (_normalMap){
		// main function tbn
		_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_TBN();
		if (_specularLighting){
			// camera tangent space
			_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_TANGENT_SPACE();
		}
	}
	else if (_specularLighting){
		// camera view space
		_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_VIEW_SPACE();
	}
	// main function lights
	_vertShaderSrc += bRenderer::shader_source_function_lightVector(_maxLights, _normalMap, _variableNumberOfLights);

	// main function end 
	_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END();
}

void ShaderDataGenerator::createFragShader()
{
	// textures
	_fragShaderSrc += bRenderer::SHADER_SOURCE_TEXTURES();
	if (_isText)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_TEXT_TEXTURES();
	// colors 
	_fragShaderSrc += bRenderer::SHADER_SOURCE_COLORS();
	// transparency value
	if (_transparencyValue)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_TRANSPARENCY_VALUE();

	// main function begin
	_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_BEGIN();
	if (_ambientLighting) {
		if (_ambientColor)
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT_COLOR();
		else
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT();
	}
	if (_diffuseLighting)
	{
		// initialize
		if (_maxLights > 0)
			_fragShaderSrc += _transparencyValue ? bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_TRANSPARENCY() : bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE();
		else
			_fragShaderSrc += _transparencyValue ? bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS_TRANSPARENCY() : bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS();
	}

	if (_specularLighting)
	{
		if (_maxLights > 0){
			if (_normalMap)
				_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_TANGENT_SPACE();
			else
				_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_VIEW_SPACE();
		}
		else
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_NO_LIGHTS();
	}

	// lighting
	if (_specularLighting || _diffuseLighting){
		if (_normalMap)
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_TANGENT_SPACE();
		else
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_VIEW_SPACE();

		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_LIGHTING();
		_fragShaderSrc += bRenderer::shader_source_function_lighting(_maxLights, _normalMap, _diffuseLighting, _specularLighting, _variableNumberOfLights);

		if (_diffuseLighting)
			_fragShaderSrc += bRenderer::shader_source_function_fragment_finalize_diffuse(_diffuseColor, _diffuseMap);

		if (_specularLighting)
			_fragShaderSrc += bRenderer::shader_source_function_fragment_finalize_specular(_specularColor, _specularMap);
	}
	

	_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_PART1();
	if (_ambientLighting)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_AMBIENT() + "+";
	if (_diffuseLighting && _specularLighting)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_DIFFUSE() + "+" + bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_SPECULAR();
	else if (_diffuseLighting)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_DIFFUSE();
	else if (_specularLighting)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_SPECULAR() + "+";
	// If alpha is not defined by diffuse lighting it is set to 1.0
	if (!_diffuseLighting)
		_fragShaderSrc += "vec4(0.0, 0.0, 0.0, 1.0)";

	_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_PART2();

	if (_isText)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_TEXT();

	_fragShaderSrc += "}";

}