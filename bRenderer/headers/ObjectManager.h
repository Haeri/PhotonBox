#ifndef B_OBJECT_MANAGER_H
#define B_OBJECT_MANAGER_H

/* bRenderer includes */
#include "Renderer_GL.h"
#include "Logger.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Configuration.h"
#include "Properties.h"
#include "Framebuffer.h"
#include "Model.h"
#include "Sprite.h"
#include "TextSprite.h"
#include "Texture.h"
#include "CubeMap.h"
#include "DepthMap.h"
#include "Font.h"
#include "ModelData.h"
#include "OBJLoader.h"
#include "TextureData.h"
#include "IShaderData.h"
#include "ShaderDataGenerator.h"

/* vmmlib includes */
#include "vmmlib/util.hpp"

/** @brief This class manages all objects in a project and makes sure no object is created twice
*	@author Benjamin Buergisser
*/
class ObjectManager
{
public:
	/* Typedefs */
	typedef std::unordered_map< std::string, ShaderPtr >		ShaderMap;
	typedef std::unordered_map< std::string, TexturePtr >		TextureMap;
	typedef std::unordered_map< std::string, CubeMapPtr >		CubeMapMap;
	typedef std::unordered_map< std::string, DepthMapPtr >		DepthMapMap;
	typedef std::unordered_map< std::string, FontPtr >			FontMap;
	typedef std::unordered_map< std::string, MaterialPtr >		MaterialMap;
	typedef std::unordered_map< std::string, PropertiesPtr >	PropertiesMap;
	typedef std::unordered_map< std::string, ModelPtr >			ModelMap;
	typedef std::unordered_map< std::string, TextSpritePtr >	TextSpriteMap;
	typedef std::unordered_map< std::string, CameraPtr >		CameraMap;
	typedef std::unordered_map< std::string, MatrixStackPtr >	MatrixStackMap;
	typedef std::unordered_map< std::string, LightPtr >			LightMap;
	typedef std::unordered_map< std::string, FramebufferPtr >	FramebufferMap;
	typedef std::unordered_map< std::string, DrawablePtr >		DrawableMap;

	/* Functions */

	/**	@brief Constructor
	*/
	ObjectManager()
	{
		_ambientColor = bRenderer::DEFAULT_AMBIENT_COLOR();
		_shaderVersionDesktop = bRenderer::DEFAULT_SHADER_VERSION_DESKTOP();
		_shaderVersionES = bRenderer::DEFAULT_SHADER_VERSION_ES();
	}

	/**	@brief Virtual destructor
	*/
	virtual ~ObjectManager(){}

	/**	@brief Set the shader version used on desktop systems
	*	@param[in] shaderVersionDesktop The shader version used on desktop systems, e.g. "#version 120"
	*/
	void setShaderVersionDesktop(const std::string &shaderVersionDesktop);

	/**	@brief Set the shader version used on mobile devices systems
	*	@param[in] shaderVersionES The shader version used on mobile devices systems, e.g. "#version 100"
	*/
	void setShaderVersionES(const std::string &shaderVersionES);

	/**	@brief Set an ambient color for the scene
	*	@param[in] ambientColor Ambient color for the scene
	*/
	void setAmbientColor(const vmml::Vector3f &ambientColor);

	/**	@brief Load a material
	*	@param[in] fileName File name including extension
	*	@param[in] materialName Name of the material
	*	@param[in] shaderName Name of the shader (optional, if left empty, the material name will be used)
	*	@param[in] maxLights The maximum number of light sources to be used (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*/
	MaterialPtr loadObjMaterial(const std::string &fileName, const std::string &materialName, const std::string &shaderName = "", GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true);

	/**	@brief Load a material
	*	@param[in] fileName File name including extension
	*	@param[in] materialName Name of the material
	*	@param[in] shader Custom shader for the model
	*/
	MaterialPtr loadObjMaterial(const std::string &fileName, const std::string &materialName, ShaderPtr shader);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture (optional)
	*	@param[in] flipZ Flip Z axis of the geometry (optional)
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded (optional)
	*	@param[in] maxLights The maximum number of light sources to be used (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*
	*	This function will automatically create one shader for every material of the model
	*
	*/
	ModelPtr loadObjModel(const std::string &fileName, bool flipT = false, bool flipZ = false, bool shaderFromFile = false, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true, PropertiesPtr properties = nullptr);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*	@param[in] shader Custom shader for the model
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr loadObjModel(const std::string &fileName, bool flipT, bool flipZ, ShaderPtr shader, PropertiesPtr	properties = nullptr);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*	@param[in] material Custom material for the model
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr loadObjModel(const std::string &fileName, bool flipT, bool flipZ, MaterialPtr material, PropertiesPtr	properties = nullptr);

	/**	@brief Load a texture
	*	@param[in] fileName File name including extension
	*/
	TexturePtr loadTexture(const std::string &fileName);

	/**	@brief Load a cube map
	*	@param[in] fileNames File names including extension (size needs to be 6) in the following order: 
	*	left, right, bottom, top, front, back
	*/
	CubeMapPtr loadCubeMap(const std::string &name, const std::vector<std::string> &fileNames);

	/**	@brief Load a font (e.g. TrueType fonts (TTF) or OpenType fonts (OTF)
	*	@param[in] fileName File name including extension
	*	@param[in] fontPixelSize The nominal  font size in pixels (the characters may vary in size)
	*/
	FontPtr loadFont(const std::string &fileName, GLuint fontPixelSize);

	/**	@brief Load a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] maxLights The maximum number of light sources to be used  (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*	@param[in] diffuseLighting Set true if the shader supports diffuse lighting (optional)
	*	@param[in] specularLighting Set true if the shader supports specular lighting (optional)
	*	@param[in] cubicReflectionMap Set true if the shader supports a cubic reflection map (optional)
	*
	*	If no shaders with the chosen name exist or no name is passed to the function
	*	the default shader will be used.
	*
	*/
	ShaderPtr loadShaderFile(const std::string &shaderName, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true, bool diffuseLighting = true, bool specularLighting = true, bool cubicReflectionMap = false);

	/**	@brief Generate a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] shaderMaxLights The maximum number of light sources to be used
	*	@param[in] ambientLighting Set true if the shader should support ambient lighting
	*	@param[in] materialData All necessary information for the shader is read from the material data
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*	@param[in] isText Set true if the shader should be used for displaying text
	*/
	ShaderPtr generateShader(const std::string &shaderName, GLuint shaderMaxLights, bool ambientLighting, const MaterialData &materialData, bool variableNumberOfLights, bool isText);

	/**	@brief Generate a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] shaderGeneratorSettings The settings defining the abilities the generated shader should have
	*/
	ShaderPtr generateShader(const std::string &shaderName, const ShaderGeneratorSettings &shaderGeneratorSettings);

	/**	@brief Create empty material
	*	@param[in] name Name of the material
	*	@param[in] shader
	*/
	MaterialPtr createMaterial(const std::string &name, ShaderPtr shader);

	/**	@brief Create a material
	*	@param[in] name Name of the material
	*	@param[in] materialData
	*	@param[in] shader
	*/
	MaterialPtr createMaterial(const std::string &name, const MaterialData &materialData, ShaderPtr shader);

	/**	@brief Create a material and a shader fitting its characteristics
	*	@param[in] name Name of the material and the shader
	*	@param[in] materialData
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded
	*	@param[in] shaderMaxLights (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*	@param[in] isText Set true if the shader should be used for displaying text (optional)
	*/
	MaterialPtr createMaterialShaderCombination(const std::string &name, const MaterialData &materialData, bool shaderFromFile, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true, bool isText = false);

	/**	@brief Create properties
	*	@param[in] name Name of the properties
	*/
	PropertiesPtr createProperties(const std::string &name);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded
	*	@param[in] maxLights The maximum number of light sources to be used (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, bool shaderFromFile, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true, PropertiesPtr properties = nullptr);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*	@param[in] shader
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, ShaderPtr shader, PropertiesPtr	properties = nullptr);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*	@param[in] material
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, MaterialPtr material, PropertiesPtr	properties = nullptr);

	/**	@brief Create a sprite
	*	@param[in] name The raw name of the sprite
	*	@param[in] material
	*	@param[in] flipT Flip T axis of texture (optional)
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createSprite(const std::string &name, MaterialPtr material, bool flipT = false, PropertiesPtr properties = nullptr);

	/**	@brief Create a sprite
	*	@param[in] name The raw name of the sprite
	*	@param[in] textureFileName	The filename of the texture that should be loaded and displayed 
	*	@param[in] shader
	*	@param[in] flipT Flip T axis of texture (optional)
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createSprite(const std::string &name, const std::string &textureFileName, ShaderPtr shader, bool flipT = false, PropertiesPtr properties = nullptr);

	/**	@brief Create a sprite
	*	@param[in] name The raw name of the sprite
	*	@param[in] textureFileName The filename of the texture that should be loaded and displayed
	*	@param[in] shaderMaxLights The maximum number of light sources to be used (optional)
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*	@param[in] flipT Flip T axis of texture (optional)
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createSprite(const std::string &name, const std::string &textureFileName, GLuint shaderMaxLights = 0, bool variableNumberOfLights = false, bool flipT = false, PropertiesPtr properties = nullptr);

	/**	@brief Create a text sprite to display strings on the screen
	*	@param[in] name The raw name of the sprite
	*	@param[in] color The color of the text
	*	@param[in] text The string to be displayed
	*	@param[in] font The font to be used to display the string
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	TextSpritePtr createTextSprite(const std::string &name, vmml::Vector3f color, const std::string &text, FontPtr font, PropertiesPtr properties = nullptr);

	/**	@brief Create a text sprite to display strings on the screen
	*	@param[in] name The raw name of the sprite
	*	@param[in] material
	*	@param[in] text The string to be displayed
	*	@param[in] font The font to be used to display the string
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	TextSpritePtr createTextSprite(const std::string &name, MaterialPtr material, const std::string &text, FontPtr font, PropertiesPtr properties = nullptr);

	/**	@brief Create a texture
	*	@param[in] name The raw name of the texture
	*	@param[in] textureData
	*/
	TexturePtr createTexture(const std::string &name, const TextureData &textureData);

	/**	@brief Create a texture (allows for creating empty textures e.g. for binding to a framebuffer)
	*	@param[in] name The raw name of the texture
	*	@param[in] width
	*	@param[in] height
	*	@param[in] format
	*	@param[in] imageData
	*/
	TexturePtr createTexture(const std::string &name, GLsizei width, GLsizei height, GLenum format = GL_RGBA, ImageDataPtr imageData = nullptr);

	/**	@brief Create a cube map
	*	@param[in] name The raw name of the cube map
	*	@param[in] data The texture data to be used (size needs to be 6) in the following order: 
	*	right, left, top, bottom, front, back
	*/
	CubeMapPtr createCubeMap(const std::string &name, const std::vector<TextureData> &data);

	/**	@brief Create a cube map
	*
	*	Allows for specifying no image data for empty cube maps (e.g. to attach to a framebuffer)
	*
	*	@param[in] name The raw name of the cube map
	*	@param[in] width Sets width and height since cube maps must use square sizes
	*	@param[in] format
	*	@param[in] imageData Image data for each face of the cube (size needs to be 6) in the following order: 
	*	right, left, top, bottom, front, back
	*/
	CubeMapPtr createCubeMap(const std::string &name, GLsizei width, GLenum format = GL_RGBA, const std::vector<ImageDataPtr> &imageData = {});

	/**	@brief Create a depth map
	*	@param[in] name The raw name of the depth map
	*	@param[in] width
	*	@param[in] height
	*/
	DepthMapPtr createDepthMap(const std::string &name, GLint width, GLint height);

	/**	@brief Create a shader
	*	@param[in] name The raw name of the shader
	*	@param[in] shaderData
	*/
	ShaderPtr createShader(const std::string &name, const IShaderData &shaderData);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*/
	CameraPtr createCamera(const std::string &name);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*	@param[in] position Position of the camera
	*	@param[in] rotationAxes Rotation axes of the camera
	*/
	CameraPtr createCamera(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	CameraPtr createCamera(const std::string &name, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*	@param[in] position Position of the camera
	*	@param[in] rotationAxes Rotation axes of the camera
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	CameraPtr createCamera(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

	/**	@brief Create a matrix stack
	*	@param[in] name Name of the matrix stack
	*/
	MatrixStackPtr createMatrixStack(const std::string &name);

	/**	@brief Create a light
	*	@param[in] name Name of the light
	*/
	LightPtr createLight(const std::string &name);

	/**	@brief Create a light
	*	@param[in] name Name of the light
	*	@param[in] position Position of the light
	*	@param[in] color Color of the light for both diffuse and specular lighting
	*/
	LightPtr createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &color);

	/**	@brief Create a light
	*	@param[in] name Name of the light
	*	@param[in] position Position of the light
	*	@param[in] color Color of the light for both diffuse and specular lighting
	*	@param[in] intensity Intensity of the light
	*	@param[in] attenuation Attenuation of the light
	*	@param[in] radius Radius of the light (clamps the light at a certain distance)
	*/
	LightPtr createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &color, GLfloat intensity, GLfloat attenuation, GLfloat radius);

	/**	@brief Create a light
	*	@param[in] name Name of the light
	*	@param[in] position Position of the light
	*	@param[in] diffuseColor Color of the light for diffuse lighting
	*	@param[in] specularColor Color of the light for specular lighting
	*	@param[in] intensity Intensity of the light
	*	@param[in] attenuation Attenuation of the light
	*	@param[in] radius Radius of the light (clamps the light at a certain distance)
	*/
	LightPtr createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &diffuseColor, const vmml::Vector3f &specularColor, GLfloat intensity, GLfloat attenuation, GLfloat radius);

	/**	@brief Create a framebuffer that adjusts its size automatically according to the viewport size
	*	@param[in] name Name of the framebuffer
	*/
	FramebufferPtr createFramebuffer(const std::string &name);
    
    /**	@brief Create a framebuffer with a custom size
     *	@param[in] name Name of the framebuffer
     *	@param[in] width
     *	@param[in] height
     */
    FramebufferPtr createFramebuffer(const std::string &name, GLint width, GLint height);

	/**	@brief Add a shader (returns true if successful)
	*	@param[in] name Name of the shader
	*/
	bool addShader(const std::string &name, ShaderPtr ptr);

	/**	@brief Add a texture (returns true if successful)
	*	@param[in] name Name of the texture
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addTexture(const std::string &name, TexturePtr ptr);

	/**	@brief Add a cube map (returns true if successful)
	*	@param[in] name Name of the cube map
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addCubeMap(const std::string &name, CubeMapPtr ptr);

	/**	@brief Add a depth map (returns true if successful)
	*	@param[in] name Name of the depth map
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addDepthMap(const std::string &name, DepthMapPtr ptr);

	/**	@brief Add a font (returns true if successful)
	*	@param[in] name Name of the font
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addFont(const std::string &name, FontPtr ptr);

	/**	@brief Add a material (returns true if successful)
	*	@param[in] name Name of the material
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addMaterial(const std::string &name, MaterialPtr ptr);

	/**	@brief Add properties (returns true if successful)
	*	@param[in] name Name of the properties
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addProperties(const std::string &name, PropertiesPtr ptr);

	/**	@brief Add a 3D model (returns true if successful)
	*	@param[in] name Name of the model
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addModel(const std::string &name, ModelPtr ptr);

	/**	@brief Add a text sprite (returns true if successful)
	*	@param[in] name Name of the text sprite
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addTextSprite(const std::string &name, TextSpritePtr ptr);

	/**	@brief Add a camera (returns true if successful)
	*	@param[in] name Name of the camera
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addCamera(const std::string &name, CameraPtr ptr);

	/**	@brief Add a matrix stack (returns true if successful)
	*	@param[in] name Name of the matrix stack
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addMatrixStack(const std::string &name, MatrixStackPtr ptr);

	/**	@brief Add a light (returns true if successful)
	*	@param[in] name Name of the light
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addLight(const std::string &name, LightPtr ptr);

	/**	@brief Add a framebuffer (returns true if successful)
	*	@param[in] name Name of the framebuffer
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addFramebuffer(const std::string &name, FramebufferPtr ptr);

	/**	@brief Add a drawable (returns true if successful)
	*	@param[in] name Name of the drawable
	*	@param[in] ptr A pointer to the object to be added
	*/
	bool addDrawable(const std::string &name, DrawablePtr ptr);

	/**	@brief Get a shader
	*	@param[in] name Name of the shader
	*/
	ShaderPtr getShader(const std::string &name);

	/**	@brief Get a texture
	*	@param[in] name Name of the texture
	*/
	TexturePtr getTexture(const std::string &name);

	/**	@brief Get a cube map
	*	@param[in] name Name of the cube map
	*/
	CubeMapPtr getCubeMap(const std::string &name);

	/**	@brief Get a depth map
	*	@param[in] name Name of the depth map
	*/
	DepthMapPtr getDepthMap(const std::string &name);

	/**	@brief Get a font
	*	@param[in] name Name of the font
	*/
	FontPtr getFont(const std::string &name);

	/**	@brief Get a material
	*	@param[in] name Name of the material
	*/
	MaterialPtr getMaterial(const std::string &name);

	/**	@brief Get properties
	*	@param[in] name Name of the properties
	*/
	PropertiesPtr getProperties(const std::string &name);

	/**	@brief Get a 3D model
	*	@param[in] name Name of the model
	*/
	ModelPtr getModel(const std::string &name);

	/**	@brief Get a text sprite
	*	@param[in] name Name of the text sprite
	*/
	TextSpritePtr getTextSprite(const std::string &name);

	/**	@brief Get a camera
	*	@param[in] name Name of the camera
	*/
	CameraPtr getCamera(const std::string &name);

	/**	@brief Get a matrix stack
	*	@param[in] name Name of the matrix stack
	*/
	MatrixStackPtr getMatrixStack(const std::string &name);

	/**	@brief Get a light
	*	@param[in] name Name of the light
	*/
	LightPtr getLight(const std::string &name);

	/**	@brief Get a framebuffer
	*	@param[in] name Name of the framebuffer
	*/
	FramebufferPtr getFramebuffer(const std::string &name);

	/**	@brief Get a drawable
	*	@param[in] name Name of the drawable
	*/
	DrawablePtr getDrawable(const std::string &name);

	/**	@brief Get the shader version used on desktop systems
	*/
	std::string getShaderVersionDesktop();

	/**	@brief Get the shader version used on mobile devices systems
	*/
	std::string getShaderVersionES();

	/**	@brief Get the ambient color of the scene
	*/
	vmml::Vector3f getAmbientColor();

	/**	@brief Remove a shader
	*	@param[in] name Name of the shader
	*	@param[in] del Set true if the OpenGL object should be deleted
	*/
	void removeShader(const std::string &name, bool del);

	/**	@brief Remove a texture
	*	@param[in] name Name of the texture
	*	@param[in] del Set true if the OpenGL object should be deleted
	*/
	void removeTexture(const std::string &name, bool del);

	/**	@brief Remove a cube map
	*	@param[in] name Name of the cube map
	*	@param[in] del Set true if the OpenGL object should be deleted
	*/
	void removeCubeMap(const std::string &name, bool del);

	/**	@brief Remove a depth map
	*	@param[in] name Name of the depth map
	*	@param[in] del Set true if the OpenGL object should be deleted
	*/
	void removeDepthMap(const std::string &name, bool del);

	/**	@brief Remove a font
	*	@param[in] name Name of the font
	*	@param[in] del Set true if the OpenGL object should be deleted
	*/
	void removeFont(const std::string &name, bool del);

	/**	@brief Remove a material
	*	@param[in] name Name of the material
	*/
	void removeMaterial(const std::string &name);

	/**	@brief Remove properties
	*	@param[in] name Name of the properties
	*/
	void removeProperties(const std::string &name);

	/**	@brief Remove a 3D model
	*	@param[in] name Name of the model
	*	@param[in] del Set true if the OpenGL object should be deleted
	*/
	void removeModel(const std::string &name, bool del);

	/**	@brief Remove a text sprite
	*	@param[in] name Name of the text sprite
	*	@param[in] del Set true if the OpenGL object should be deleted
	*/
	void removeTextSprite(const std::string &name, bool del);

	/**	@brief Remove a camera
	*	@param[in] name Name of the camera
	*/
	void removeCamera(const std::string &name);

	/**	@brief Remove a matrix stack
	*	@param[in] name Name of the matrix stack
	*/
	void removeMatrixStack(const std::string &name);

	/**	@brief Remove a light
	*	@param[in] name Name of the light
	*/
	void removeLight(const std::string &name);

	/**	@brief Remove a framebuffer
	*	@param[in] name Name of the framebuffer
	*	@param[in] del Set true if the OpenGL object should be deleted
	*/
	void removeFramebuffer(const std::string &name, bool del);

	/**	@brief Remove a drawable
	*	@param[in] name Name of the drawable
	*/
	void removeDrawable(const std::string &name);

	/**	@brief Clear all objects
	*	@param[in] del Set true if the OpenGL objects should be deleted
	*/
	void clear(bool del);

private:
	/* Functions */

	/**	@brief Get the name of a file from the filename
	*	@param[in] fileName The filename
	*	@param[in] ext The extension
	*/
	std::string getRawName(const std::string &fileName, std::string *ext = nullptr);

	/* Variables */

	ShaderMap		_shaders;
	TextureMap		_textures;
	CubeMapMap		_cubeMaps;
	DepthMapMap		_depthMaps;
	FontMap			_fonts;
	MaterialMap		_materials;
	PropertiesMap	_properties;
	ModelMap	    _models;
	TextSpriteMap	_textSprites;
	CameraMap		_cameras;
	MatrixStackMap	_matrixStacks;
	LightMap		_lights;
	FramebufferMap	_framebuffers;
	DrawableMap		_drawables;

	vmml::Vector3f		_ambientColor;

	std::string		_shaderVersionDesktop;
	std::string		_shaderVersionES;

};

typedef std::shared_ptr< ObjectManager >  ObjectManagerPtr;

#endif /* defined(B_OBJECT_MANAGER_H) */
