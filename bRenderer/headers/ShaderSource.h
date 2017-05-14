#ifndef B_SHADER_SOURCE_H
#define B_SHADER_SOURCE_H

#include "Configuration.h"

namespace bRenderer
{
	std::string SHADER_SOURCE_LINE_BREAK();
	std::string SHADER_SOURCE_LINE_ENDING();
	std::string SHADER_SOURCE_WHITE_SPACE();

	// Head
	std::string SHADER_SOURCE_HEAD_ES();
	std::string SHADER_SOURCE_HEAD_DESKTOP();

	// Lights
	std::string SHADER_SOURCE_NUM_LIGHTS();

    std::string shader_source_light_properties(GLuint maxLights, bool normalMap, bool diffuseLighting, bool specularLighting);

	// Matrices
	std::string SHADER_SOURCE_MATRICES();

	// Attributes
	std::string SHADER_SOURCE_ATTRIBUTES();

	// Varyings
	std::string SHADER_SOURCE_VARYINGS_TEX_COORD();

	std::string SHADER_SOURCE_VARYINGS_NORMAL();

	std::string SHADER_SOURCE_VARYINGS_CAMERA_TANGENT();

	std::string SHADER_SOURCE_VARYINGS_CAMERA_VIEW();

	// Colors 
	std::string SHADER_SOURCE_COLORS();

	// Transparency value 
	std::string SHADER_SOURCE_TRANSPARENCY_VALUE();

	// Textures
	std::string SHADER_SOURCE_TEXTURES();

	// Text Textures
	std::string SHADER_SOURCE_TEXT_TEXTURES();

	/* Vertex Shader */

	// Vertex Shader Main Function
	//Begin
	std::string shader_source_function_vertex_main_begin(bool hasLighting, bool hasTextures, bool normalMap);

	// TBN for normal map
	std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_TBN();
	// Camera tangent space
	std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_TANGENT_SPACE();
	// Camera view space
	std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_VIEW_SPACE();
	// Lighting
    std::string shader_source_function_lightVector(GLuint maxLights, bool normalMap, bool variableNumberOfLights);
	
	// End
	std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END();

	/* Fragment Shader */

	// Fragment Shader Main Function
	// Begin
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_BEGIN();

	// Ambient
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT();
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT_COLOR();

	// Normals
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_TANGENT_SPACE();
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_VIEW_SPACE();

	// Initialize diffuse lighting
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE();
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS();
	// Initialize diffuse lighting with transparency value
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_TRANSPARENCY();
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS_TRANSPARENCY();

	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_LIGHTING();

	// Initialize specular lighting
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_VIEW_SPACE();

	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_TANGENT_SPACE();

	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_NO_LIGHTS();
	
	// Lighting
    std::string shader_source_function_lighting(GLuint maxLights, bool normalMap, bool diffuseLighting, bool specularLighting, bool variableNumberOfLights);

	// Finalize diffuse lighting
    std::string shader_source_function_fragment_finalize_diffuse(bool diffuseColor, bool diffuseMap);

	// Finalize specular lighting
    std::string shader_source_function_fragment_finalize_specular(bool specularColor, bool specularMap);

	// End
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_PART1();
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_AMBIENT();
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_DIFFUSE();
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_SPECULAR();
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_PART2();

	// End Text
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_TEXT();


} // namespace bRenderer

#endif /* defined(B_SHADER_SOURCE_H) */