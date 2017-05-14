#include "headers/ShaderSource.h"

namespace bRenderer
{
    std::string SHADER_SOURCE_LINE_BREAK() { static std::string x = "\n"; return x; }
    std::string SHADER_SOURCE_LINE_ENDING() { return ";" + SHADER_SOURCE_LINE_BREAK(); }
    std::string SHADER_SOURCE_WHITE_SPACE() { return " "; }
    
    // Head
	std::string SHADER_SOURCE_HEAD_ES() { return "#version 100" + SHADER_SOURCE_LINE_BREAK() + "precision mediump float" + SHADER_SOURCE_LINE_ENDING(); }
	std::string SHADER_SOURCE_HEAD_DESKTOP() { return "#version 120" + SHADER_SOURCE_LINE_BREAK(); }
    
    // Lights
    std::string SHADER_SOURCE_NUM_LIGHTS() { return "uniform float " + DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS() + SHADER_SOURCE_LINE_ENDING(); }
    
    std::string shader_source_light_properties(GLuint maxLights, bool normalMap, bool diffuseLighting, bool specularLighting)
    {
        std::string lighting = "";
        for (GLuint light_number = 0; light_number < maxLights; light_number++){
            std::string num = std::to_string(light_number);
            lighting += "uniform vec4 " + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE() + num + SHADER_SOURCE_LINE_ENDING()
				+ "uniform float " + DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY() + num + SHADER_SOURCE_LINE_ENDING()
				+ "uniform float " + DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION() + num + SHADER_SOURCE_LINE_ENDING()
				+ "uniform float " + DEFAULT_SHADER_UNIFORM_LIGHT_RADIUS() + num + SHADER_SOURCE_LINE_ENDING();
            if (true)
				lighting += "uniform vec3 " + DEFAULT_SHADER_UNIFORM_DIFFUSE_LIGHT_COLOR() + num + SHADER_SOURCE_LINE_ENDING();
            if (true)
				lighting += "uniform vec3 " + DEFAULT_SHADER_UNIFORM_SPECULAR_LIGHT_COLOR() + num + SHADER_SOURCE_LINE_ENDING();
            if (normalMap)
                lighting += "varying vec3 lightVectorTangentSpace_" + num + SHADER_SOURCE_LINE_ENDING();
            else
                lighting += "varying vec3 lightVectorViewSpace_" + num + SHADER_SOURCE_LINE_ENDING();
			lighting += "varying float intensityBasedOnDist_" + num + SHADER_SOURCE_LINE_ENDING();
        }
        return lighting;
    }
    
    // Matrices
	std::string SHADER_SOURCE_MATRICES() { return
		"uniform mat4 " + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX() + SHADER_SOURCE_LINE_ENDING()
		+ "uniform mat4 " + DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX() + SHADER_SOURCE_LINE_ENDING(); }

    // Attributes
    std::string SHADER_SOURCE_ATTRIBUTES() { return
    "attribute vec4 " + DEFAULT_SHADER_ATTRIBUTE_POSITION() + SHADER_SOURCE_LINE_ENDING()
	+ "attribute vec3 " + DEFAULT_SHADER_ATTRIBUTE_NORMAL() + SHADER_SOURCE_LINE_ENDING()
	+ "attribute vec3 " + DEFAULT_SHADER_ATTRIBUTE_TANGENT() + SHADER_SOURCE_LINE_ENDING()
	+ "attribute vec3 " + DEFAULT_SHADER_ATTRIBUTE_BITANGENT() + SHADER_SOURCE_LINE_ENDING()
	+ "attribute vec4 " + DEFAULT_SHADER_ATTRIBUTE_TEXCOORD() + SHADER_SOURCE_LINE_ENDING(); }
    
    // Varyings
    std::string SHADER_SOURCE_VARYINGS_TEX_COORD() { return "varying vec2 texCoordVarying" + SHADER_SOURCE_LINE_ENDING(); }
    
    std::string SHADER_SOURCE_VARYINGS_NORMAL() { return "varying vec3 normalVaryingViewSpace" + SHADER_SOURCE_LINE_ENDING(); }
    
    std::string SHADER_SOURCE_VARYINGS_CAMERA_TANGENT() { return "varying vec3 surfaceToCameraTangentSpace" + SHADER_SOURCE_LINE_ENDING(); }

	std::string SHADER_SOURCE_VARYINGS_CAMERA_VIEW() { return "varying vec3 surfaceToCameraViewSpace" + SHADER_SOURCE_LINE_ENDING(); }
    
    // Colors
    std::string SHADER_SOURCE_COLORS() { return
		"uniform vec3 " + DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR() + SHADER_SOURCE_LINE_ENDING()
		+ "uniform vec3 " + WAVEFRONT_MATERIAL_AMBIENT_COLOR() + SHADER_SOURCE_LINE_ENDING()
		+ "uniform vec3 " + WAVEFRONT_MATERIAL_DIFFUSE_COLOR() + SHADER_SOURCE_LINE_ENDING()
		+ "uniform vec3 " + WAVEFRONT_MATERIAL_SPECULAR_COLOR() + SHADER_SOURCE_LINE_ENDING()
		+ "uniform float " + WAVEFRONT_MATERIAL_SPECULAR_EXPONENT() + SHADER_SOURCE_LINE_ENDING(); }

	// Transparency value 
	std::string SHADER_SOURCE_TRANSPARENCY_VALUE() { return "uniform float " + DEFAULT_SHADER_UNIFORM_TRANSPARENCY() + SHADER_SOURCE_LINE_ENDING(); }
    
    // Textures
    std::string SHADER_SOURCE_TEXTURES() { return
		"uniform sampler2D " + DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP() + SHADER_SOURCE_LINE_ENDING()
		+ "uniform sampler2D " + DEFAULT_SHADER_UNIFORM_NORMAL_MAP() + SHADER_SOURCE_LINE_ENDING()
		+ "uniform sampler2D " + DEFAULT_SHADER_UNIFORM_SPECULAR_MAP() + SHADER_SOURCE_LINE_ENDING(); }
    // Text Textures
	std::string SHADER_SOURCE_TEXT_TEXTURES() { return  "uniform sampler2D " + DEFAULT_SHADER_UNIFORM_CHARACTER_MAP() + SHADER_SOURCE_LINE_ENDING(); }
    /* Vertex Shader */
    
    // Vertex Shader Main Function
    //Begin
	std::string shader_source_function_vertex_main_begin(bool hasLighting, bool hasTextures, bool normalMap)
    {
		std::string main =
			"void main() {" + SHADER_SOURCE_LINE_BREAK()
			+ "vec4 posViewSpace = " + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX() + "*" + DEFAULT_SHADER_ATTRIBUTE_POSITION() + SHADER_SOURCE_LINE_ENDING()
			+ "float lightDistance = 0.0" + SHADER_SOURCE_LINE_ENDING();
        if (hasTextures)
			main += "texCoordVarying = " + DEFAULT_SHADER_ATTRIBUTE_TEXCOORD() + ".st" + SHADER_SOURCE_LINE_ENDING();
        if (hasLighting && !normalMap)
			main += "normalVaryingViewSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX() + ")*" + DEFAULT_SHADER_ATTRIBUTE_NORMAL() + SHADER_SOURCE_LINE_ENDING();
        return main;
    }
    // TBN for normal map
    std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_TBN() { return
		+"vec3 vertexNormal_ViewSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX() + ") * " + DEFAULT_SHADER_ATTRIBUTE_NORMAL() + SHADER_SOURCE_LINE_ENDING()
		+ "vec3 vertexTangent_ViewSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX() + ") * " + DEFAULT_SHADER_ATTRIBUTE_TANGENT() + SHADER_SOURCE_LINE_ENDING()
		+ "vec3 vertexBitangent_ViewSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX() + ") * " + DEFAULT_SHADER_ATTRIBUTE_BITANGENT() + SHADER_SOURCE_LINE_ENDING()
    + "vec3 firstRow = vec3(vertexTangent_ViewSpace.x, vertexBitangent_ViewSpace.x, vertexNormal_ViewSpace.x)" + SHADER_SOURCE_LINE_ENDING()
    + "vec3 secondRow = vec3(vertexTangent_ViewSpace.y, vertexBitangent_ViewSpace.y, vertexNormal_ViewSpace.y)" + SHADER_SOURCE_LINE_ENDING()
    + "vec3 thirdRow = vec3(vertexTangent_ViewSpace.z, vertexBitangent_ViewSpace.z, vertexNormal_ViewSpace.z)" + SHADER_SOURCE_LINE_ENDING()
    + "mat3 TBN = mat3(firstRow, secondRow, thirdRow)" + SHADER_SOURCE_LINE_ENDING(); }
    // Camera tangent space
    std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_TANGENT_SPACE() { return "surfaceToCameraTangentSpace = TBN*( - posViewSpace.xyz )" + SHADER_SOURCE_LINE_ENDING(); }
	// Camera view space
	std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_VIEW_SPACE() { return "surfaceToCameraViewSpace = -posViewSpace.xyz" + SHADER_SOURCE_LINE_ENDING(); }
    // Lighting
    std::string shader_source_function_lightVector(GLuint maxLights, bool normalMap, bool variableNumberOfLights)
    {
        std::string lighting = "";
		for (GLuint light_number = 0; light_number < maxLights; light_number++){
			std::string num = std::to_string(light_number);
			std::string numPP = std::to_string(light_number + 1);
			if (variableNumberOfLights)
				lighting += "if(" + DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS() + " >= " + numPP + ".0){" + SHADER_SOURCE_LINE_BREAK();
			if (normalMap)
				lighting += "lightVectorTangentSpace_" + num + " = TBN*(" + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE() + num + ".xyz - posViewSpace.xyz)" + SHADER_SOURCE_LINE_ENDING();
			else
				lighting += "lightVectorViewSpace_" + num + " = " + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE() + num + ".xyz - posViewSpace.xyz" + SHADER_SOURCE_LINE_ENDING();
			lighting += "lightDistance = distance(posViewSpace, " + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE() + num + ")" + SHADER_SOURCE_LINE_ENDING()
				+ "intensityBasedOnDist_" + num + " = 0.0" + SHADER_SOURCE_LINE_ENDING()
				+ "if (lightDistance <= " + DEFAULT_SHADER_UNIFORM_LIGHT_RADIUS() + num + ") {" + SHADER_SOURCE_LINE_BREAK()
				+ "intensityBasedOnDist_" + num + " = clamp(" + DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY() + num + " / (" + DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION() + num + "*lightDistance*lightDistance), 0.0, 1.0)" + SHADER_SOURCE_LINE_ENDING()
				+ "}" + SHADER_SOURCE_LINE_BREAK();
		}
        if (variableNumberOfLights){
            for (GLuint light_number = 0; light_number < maxLights; light_number++)
                lighting += "} ";
        }
        return lighting;
    }
    // End
    std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END() { return
		+"gl_Position = " + DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX() + "*posViewSpace" + SHADER_SOURCE_LINE_ENDING()
    + "}" + SHADER_SOURCE_LINE_BREAK(); }
    
    /* Fragment Shader */
    
    // Fragment Shader Main Function
    // Begin
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_BEGIN() { return "void main() {" + SHADER_SOURCE_LINE_BREAK(); }
    
    // Ambient
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT() { return "vec4 ambient  = vec4(clamp(" + DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR() + ", 0.0, 1.0), 0.0)" + SHADER_SOURCE_LINE_ENDING(); }
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT_COLOR() { return "vec4 ambient  = vec4(clamp(" + DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR() + "*" + WAVEFRONT_MATERIAL_AMBIENT_COLOR() + ", 0.0, 1.0), 0.0)" + SHADER_SOURCE_LINE_ENDING(); }
    
    // Normals
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_TANGENT_SPACE() { return "vec3 surfaceNormal = normalize(texture2D(" + DEFAULT_SHADER_UNIFORM_NORMAL_MAP() + ", texCoordVarying).xyz *2.0 - 1.0)" + SHADER_SOURCE_LINE_ENDING(); }
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_VIEW_SPACE() { return "vec3 surfaceNormal = normalize(normalVaryingViewSpace)" + SHADER_SOURCE_LINE_ENDING(); }
    
    // Initialize diffuse lighting
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE() { return  "vec4 diffuse = vec4(0.0,0.0,0.0,1.0)" + SHADER_SOURCE_LINE_ENDING(); }    
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS() { return "vec4 diffuse = vec4(1.0,1.0,1.0,1.0)" + SHADER_SOURCE_LINE_ENDING(); }
	// Initialize diffuse lighting with transparency value
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_TRANSPARENCY() { return "vec4 diffuse = vec4(0.0,0.0,0.0," + DEFAULT_SHADER_UNIFORM_TRANSPARENCY() + ")" + SHADER_SOURCE_LINE_ENDING(); }
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS_TRANSPARENCY() { return "vec4 diffuse = vec4(1.0,1.0,1.0," + DEFAULT_SHADER_UNIFORM_TRANSPARENCY() + ")" + SHADER_SOURCE_LINE_ENDING(); }

    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_LIGHTING() { return "float intensity = 0.0" + SHADER_SOURCE_LINE_ENDING(); }
    
    // Initialize specular lighting
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_VIEW_SPACE() { return
    "vec4 specular = vec4(0.0,0.0,0.0,0.0)" + SHADER_SOURCE_LINE_ENDING()
    + "float specularCoefficient = 0.0" + SHADER_SOURCE_LINE_ENDING()
	+ "vec3 surfaceToCamera = normalize(surfaceToCameraViewSpace)" + SHADER_SOURCE_LINE_ENDING(); }
    
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_TANGENT_SPACE() { return
    "vec4 specular = vec4(0.0,0.0,0.0,0.0)" + SHADER_SOURCE_LINE_ENDING()
    + "float specularCoefficient = 0.0" + SHADER_SOURCE_LINE_ENDING()
	+ "vec3 surfaceToCamera = normalize(surfaceToCameraTangentSpace)" + SHADER_SOURCE_LINE_ENDING(); }

	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_NO_LIGHTS() { return "vec4 specular = vec4(0.0,0.0,0.0,0.0)" + SHADER_SOURCE_LINE_ENDING(); }
    
    // Lighting
    std::string shader_source_function_lighting(GLuint maxLights, bool normalMap, bool diffuseLighting, bool specularLighting, bool variableNumberOfLights)
    {
        std::string lighting = "";
        for (GLuint light_number = 0; light_number < maxLights; light_number++){
            std::string num = std::to_string(light_number);
            std::string numPP = std::to_string(light_number + 1);
            if (variableNumberOfLights)
				lighting += "if(" + DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS() + " >= " + numPP + ".0){" + SHADER_SOURCE_LINE_BREAK();
            if (normalMap)
				lighting += "if (intensityBasedOnDist_" + num + " > 0.0 && (intensity = max(dot(surfaceNormal, normalize(lightVectorTangentSpace_" + num + ")), 0.0)) > 0.0){" + SHADER_SOURCE_LINE_BREAK();
            else
				lighting += "if (intensityBasedOnDist_" + num + " > 0.0 && (intensity = max(dot(surfaceNormal, normalize(lightVectorViewSpace_" + num + ")), 0.0)) > 0.0){" + SHADER_SOURCE_LINE_BREAK();
			
			lighting += "intensity = clamp(intensity, 0.0, 1.0)" + SHADER_SOURCE_LINE_ENDING();
            if (diffuseLighting)
				lighting += "diffuse += vec4(" + DEFAULT_SHADER_UNIFORM_DIFFUSE_LIGHT_COLOR() + num + " * (intensity * intensityBasedOnDist_" + num + "), 0.0)" + SHADER_SOURCE_LINE_ENDING();
            if (specularLighting) {
                if (normalMap)
                    lighting += "specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-normalize(lightVectorTangentSpace_" + num + "), surfaceNormal))), ";
                else
                    lighting += "specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-normalize(lightVectorViewSpace_" + num + "), surfaceNormal))), ";
				lighting += WAVEFRONT_MATERIAL_SPECULAR_EXPONENT() + ")" + SHADER_SOURCE_LINE_ENDING();
				lighting += "specular += vec4(" + DEFAULT_SHADER_UNIFORM_SPECULAR_LIGHT_COLOR() + num;
				lighting += " * (specularCoefficient * intensity * intensityBasedOnDist_" + num + "), 0.0)" + SHADER_SOURCE_LINE_ENDING();
            }
            lighting += "}";
        }
        if (variableNumberOfLights){
            for (GLuint light_number = 0; light_number < maxLights; light_number++)
                lighting += "} ";
        }
        return lighting;
    }
    
    // Finalize diffuse lighting
    std::string shader_source_function_fragment_finalize_diffuse(bool diffuseColor, bool diffuseMap)
    {
        std::string diffuse = "diffuse = diffuse";
        if (diffuseColor)
			diffuse += " * vec4(" + WAVEFRONT_MATERIAL_DIFFUSE_COLOR() + ",1.0)";
        if (diffuseMap)
			diffuse += " * texture2D(" + DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP() + ", texCoordVarying)";
        return diffuse + SHADER_SOURCE_LINE_ENDING();
    }
    
    // Finalize specular lighting
    std::string shader_source_function_fragment_finalize_specular(bool specularColor, bool specularMap)
    {
        std::string specular = "specular = specular ";
        if (specularColor)
			specular += " * vec4(" + WAVEFRONT_MATERIAL_SPECULAR_COLOR() + ", 0.0)";
        if (specularMap)
			specular += " * texture2D(" + DEFAULT_SHADER_UNIFORM_SPECULAR_MAP() + ", texCoordVarying)";
        return specular + SHADER_SOURCE_LINE_ENDING();
    }
    
    // End
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_PART1() { return "gl_FragColor = clamp("; }
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_AMBIENT() { return "ambient"; }
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_DIFFUSE() { return "diffuse"; }
    std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_SPECULAR() { return "specular"; }
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_PART2() { return ", 0.0, 1.0)" + SHADER_SOURCE_LINE_ENDING(); }
	// End Text
	std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_TEXT() { return "gl_FragColor.a *= texture2D(" + DEFAULT_SHADER_UNIFORM_CHARACTER_MAP() + ", texCoordVarying).r" + SHADER_SOURCE_LINE_ENDING(); }
    
} // namespace bRenderer

