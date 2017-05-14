#include "headers/Configuration.h"
#include "external/jsoncpp/json.h"
#include "headers/Logger.h"
#include "headers/FileHandler.h"
#include <fstream>

namespace bRenderer
{
	Json::Value jsonValue;   // will contains the root value after parsing.
	Json::Reader jsonReader;
	
	bool loadConfigFile(const std::string &fileName){
		std::ifstream file(getFilePath(fileName), std::ifstream::in);
		if (!jsonReader.parse(file, jsonValue))
		{
			log("Failed to parse configurations file: " + jsonReader.getFormattedErrorMessages(), LM_ERROR);
			return false;
		}
		return true;
	}

	/* Renderer configuration */
	vmml::Vector3f DEFAULT_AMBIENT_COLOR() { 
		const Json::Value x = jsonValue["DEFAULT_AMBIENT_COLOR"];
		return vmml::Vector3f(x[0].asFloat(), x[1].asFloat(), x[2].asFloat());
	}

	/* Logger */
	std::string LOG_MODE_INFO() { return jsonValue.get("LOG_MODE_INFO", "INFO").asString(); }
	std::string LOG_MODE_WARNING() { return jsonValue.get("LOG_MODE_WARNING", "WARNING").asString(); }
	std::string LOG_MODE_ERROR() { return jsonValue.get("LOG_MODE_ERROR", "ERROR").asString(); }
	std::string LOG_MODE_SYSTEM() { return jsonValue.get("LOG_MODE_SYSTEM", "SYSTEM").asString(); }

	/* Desktop specific view configuration */
	GLint DEFAULT_VIEW_WIDTH() { return jsonValue.get("DEFAULT_VIEW_WIDTH", 640).asInt(); }
	GLint DEFAULT_VIEW_HEIGHT() { return jsonValue.get("DEFAULT_VIEW_HEIGHT", 480).asInt(); }
	std::string DEFAULT_WINDOW_TITLE() { return jsonValue.get("DEFAULT_WINDOW_TITLE", "bRenderer").asString(); }

	/* Desktop specific file path configuration */
	std::string DEFAULT_FILE_PATH() { return jsonValue.get("DEFAULT_FILE_PATH", "data/").asString(); }

	/* OBJ loader default group name */
	std::string DEFAULT_GROUP_NAME() { return jsonValue.get("DEFAULT_GROUP_NAME", "default").asString(); }

	/* Shader macros*/
	std::string SHADER_VERSION_MACRO() { return jsonValue.get("SHADER_VERSION_MACRO", "$B_SHADER_VERSION").asString(); }
	std::string SHADER_MAX_LIGHTS_MACRO() { return jsonValue.get("SHADER_MAX_LIGHTS_MACRO", "$B_SHADER_MAX_LIGHTS").asString(); }

	/* Shader configuration */
	std::string DEFAULT_SHADER_VERSION_DESKTOP() { return jsonValue.get("DEFAULT_SHADER_VERSION_DESKTOP", "#version 120").asString(); }
	std::string DEFAULT_SHADER_VERSION_ES() { return jsonValue.get("DEFAULT_SHADER_VERSION_ES", "#version 100").asString(); }
	GLuint DEFAULT_SHADER_MAX_LIGHTS() { return jsonValue.get("DEFAULT_SHADER_MAX_LIGHTS", 3).asUInt(); }
	std::string DEFAULT_FRAGMENT_SHADER_FILENAME_EXTENSION() { return jsonValue.get("DEFAULT_FRAGMENT_SHADER_FILENAME_EXTENSION", ".frag").asString(); }
	std::string DEFAULT_VERTEX_SHADER_FILENAME_EXTENSION() { return jsonValue.get("DEFAULT_VERTEX_SHADER_FILENAME_EXTENSION", ".vert").asString(); }

	/* Shader uniforms */
	std::string DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP", "DiffuseMap").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_SPECULAR_MAP() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_SPECULAR_MAP", "SpecularMap").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_NORMAL_MAP() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_NORMAL_MAP", "NormalMap").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_CUBE_MAP() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_CUBE_MAP", "CubeMap").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_SPHERE_MAP() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_SPHERE_MAP", "SphereMap").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_CHARACTER_MAP() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_CHARACTER_MAP", "CharacterMap").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_NORMAL_MATRIX() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_NORMAL_MATRIX", "NormalMatrix").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_MODEL_MATRIX() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_MODEL_MATRIX", "ModelMatrix").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_VIEW_MATRIX() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_VIEW_MATRIX", "ViewMatrix").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX", "ModelViewMatrix").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_INVERSE_VIEW_MATRIX()  { return jsonValue.get("DEFAULT_SHADER_UNIFORM_INVERSE_VIEW_MATRIX", "InverseViewMatrix").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX", "ProjectionMatrix").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS", "numLights").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE", "lightPositionViewSpace_").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_DIFFUSE_LIGHT_COLOR() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_DIFFUSE_LIGHT_COLOR", "lightDiffuseColor_").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_SPECULAR_LIGHT_COLOR() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_SPECULAR_LIGHT_COLOR", "lightSpecularColor_").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY", "lightIntensity_").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION", "lightAttenuation_").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_LIGHT_RADIUS() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_LIGHT_RADIUS", "lightRadius_").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR", "ambientColor").asString(); }
	std::string DEFAULT_SHADER_UNIFORM_TRANSPARENCY() { return jsonValue.get("DEFAULT_SHADER_UNIFORM_TRANSPARENCY", "transparency").asString(); }

	/* Shader attributes */
	std::string DEFAULT_SHADER_ATTRIBUTE_POSITION() { return jsonValue.get("DEFAULT_SHADER_ATTRIBUTE_POSITION", "Position").asString(); }
	std::string DEFAULT_SHADER_ATTRIBUTE_NORMAL() { return jsonValue.get("DEFAULT_SHADER_ATTRIBUTE_NORMAL", "Normal").asString(); }
	std::string DEFAULT_SHADER_ATTRIBUTE_TANGENT() { return jsonValue.get("DEFAULT_SHADER_ATTRIBUTE_TANGENT", "Tangent").asString(); }
	std::string DEFAULT_SHADER_ATTRIBUTE_BITANGENT() { return jsonValue.get("DEFAULT_SHADER_ATTRIBUTE_BITANGENT", "Bitangent").asString(); }
	std::string DEFAULT_SHADER_ATTRIBUTE_TEXCOORD() { return jsonValue.get("DEFAULT_SHADER_ATTRIBUTE_TEXCOORD", "TexCoord").asString(); }

	/* Material key words */
	std::string WAVEFRONT_MATERIAL_NEWMTL() { return jsonValue.get("WAVEFRONT_MATERIAL_NEWMTL", "newmtl").asString(); }
	std::string WAVEFRONT_MATERIAL_AMBIENT_COLOR() { return jsonValue.get("WAVEFRONT_MATERIAL_AMBIENT_COLOR", "Ka").asString(); }
	std::string WAVEFRONT_MATERIAL_DIFFUSE_COLOR() { return jsonValue.get("WAVEFRONT_MATERIAL_DIFFUSE_COLOR", "Kd").asString(); }
	std::string WAVEFRONT_MATERIAL_SPECULAR_COLOR() { return jsonValue.get("WAVEFRONT_MATERIAL_SPECULAR_COLOR", "Ks").asString(); }
	std::string WAVEFRONT_MATERIAL_TRANSMISSION_FILTER() { return jsonValue.get("WAVEFRONT_MATERIAL_TRANSMISSION_FILTER", "Tf").asString(); }
	std::string WAVEFRONT_MATERIAL_SPECULAR_EXPONENT() { return jsonValue.get("WAVEFRONT_MATERIAL_SPECULAR_EXPONENT", "Ns").asString(); }
	std::string WAVEFRONT_MATERIAL_OPTICAL_DENSITY() { return jsonValue.get("WAVEFRONT_MATERIAL_OPTICAL_DENSITY", "Ni").asString(); }
	std::string WAVEFRONT_MATERIAL_ILLUMINATION_MODEL() { return jsonValue.get("WAVEFRONT_MATERIAL_ILLUMINATION_MODEL", "illum").asString(); }
	std::string WAVEFRONT_MATERIAL_DIFFUSE_MAP() { return jsonValue.get("WAVEFRONT_MATERIAL_DIFFUSE_MAP", "map_Kd").asString(); }
	std::string WAVEFRONT_MATERIAL_SPECULAR_MAP() { return jsonValue.get("WAVEFRONT_MATERIAL_SPECULAR_MAP", "map_Ks").asString(); }
	std::string WAVEFRONT_MATERIAL_NORMAL_MAP() { return jsonValue.get("WAVEFRONT_MATERIAL_NORMAL_MAP", "map_Bump").asString(); }
	std::string WAVEFRONT_MATERIAL_DISSOLVE() { return jsonValue.get("WAVEFRONT_MATERIAL_DISSOLVE", "d").asString(); }
	std::string WAVEFRONT_MATERIAL_REFLECTION() { return jsonValue.get("WAVEFRONT_MATERIAL_REFLECTION", "refl").asString(); }
	std::string WAVEFRONT_MATERIAL_REFLECTION_TYPE() { return jsonValue.get("WAVEFRONT_MATERIAL_REFLECTION_TYPE", "-type").asString(); }
	std::string WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_TOP() { return jsonValue.get("WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_TOP", "cube_top").asString(); }
	std::string WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_BOTTOM() { return jsonValue.get("WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_BOTTOM", "cube_bottom").asString(); }
	std::string WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_FRONT() { return jsonValue.get("WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_FRONT", "cube_front").asString(); }
	std::string WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_BACK() { return jsonValue.get("WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_BACK", "cube_back").asString(); }
	std::string WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_LEFT() { return jsonValue.get("WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_LEFT", "cube_left").asString(); }
	std::string WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_RIGHT() { return jsonValue.get("WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_RIGHT", "cube_right").asString(); }
	std::string WAVEFRONT_MATERIAL_REFLECTION_TYPE_SPHERE() { return jsonValue.get("WAVEFRONT_MATERIAL_REFLECTION_TYPE_SPHERE", "sphere").asString(); }

	/* Camera configuration */
	GLfloat DEFAULT_FIELD_OF_VIEW() { return jsonValue.get("DEFAULT_FIELD_OF_VIEW", 60.0f).asFloat(); }
	GLfloat DEFAULT_ASPECT_RATIO() { return jsonValue.get("DEFAULT_ASPECT_RATIO", 4.0f / 3.0f).asFloat(); }
	GLfloat DEFAULT_NEAR_CLIPPING_PLANE() { return jsonValue.get("DEFAULT_NEAR_CLIPPING_PLANE", -1.0f).asFloat(); }
	GLfloat DEFAULT_FAR_CLIPPING_PLANE() { return jsonValue.get("DEFAULT_FAR_CLIPPING_PLANE", 1.0f).asFloat(); }
	vmml::Vector3f DEFAULT_CAMERA_POSITION() {
		const Json::Value x = jsonValue["DEFAULT_CAMERA_POSITION"];
		return vmml::Vector3f(x[0].asFloat(), x[1].asFloat(), x[2].asFloat());
	}
	vmml::Vector3f DEFAULT_CAMERA_ROTATION_AXES() {
		const Json::Value x = jsonValue["DEFAULT_CAMERA_ROTATION_AXES"];
		return vmml::Vector3f(x[0].asFloat(), x[1].asFloat(), x[2].asFloat());
	}

	/* Light configuration */
	vmml::Vector4f DEFAULT_LIGHT_POSITION()	{
		const Json::Value x = jsonValue["DEFAULT_LIGHT_POSITION"];
		return vmml::Vector4f(x[0].asFloat(), x[1].asFloat(), x[2].asFloat(), 1.0);
	}
	vmml::Vector3f DEFAULT_LIGHT_COLOR() { 
		const Json::Value x = jsonValue["DEFAULT_LIGHT_COLOR"];
		if (x.size() >= 3)
			return vmml::Vector3f(x[0].asFloat(), x[1].asFloat(), x[2].asFloat());
		static const vmml::Vector3f y = vmml::Vector3f(1.0f, 1.0f, 1.0f); 
		return y; 
	}
	GLfloat DEFAULT_LIGHT_INTENSITY()		{ return jsonValue.get("DEFAULT_LIGHT_INTENSITY", 1000.0f).asFloat(); }
	GLfloat DEFAULT_LIGHT_ATTENUATION()		{ return jsonValue.get("DEFAULT_LIGHT_ATTENUATION", 1.0f).asFloat(); }
	GLfloat DEFAULT_LIGHT_RADIUS()			{ return jsonValue.get("DEFAULT_LIGHT_RADIUS", 10000.0f).asFloat(); }

	/* Font configuration */
	GLuint FONT_MAX_PIXEL_SIZE()			{ return jsonValue.get("FONT_MAX_PIXEL_SIZE", 512).asUInt(); }

} // namespace bRenderer
