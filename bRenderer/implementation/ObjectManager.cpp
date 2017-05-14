#include "headers/ObjectManager.h"
#include "headers/ShaderDataFile.h"
#include "headers/ShaderDataGenerator.h"

/* Public functions */

void ObjectManager::setShaderVersionDesktop(const std::string &shaderVersionDesktop)
{
	_shaderVersionDesktop = shaderVersionDesktop;
}

void ObjectManager::setShaderVersionES(const std::string &shaderVersionES)
{
	_shaderVersionES = shaderVersionES;
}

void ObjectManager::setAmbientColor(const vmml::Vector3f &ambientColor)
{
	_ambientColor = ambientColor;
}

MaterialPtr ObjectManager::loadObjMaterial(const std::string &fileName, const std::string &materialName, const std::string &shaderName, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting)
{
	// log activity
	bRenderer::log("loading Material: " + materialName, bRenderer::LM_SYS);

	if (getModel(materialName))
		return _materials[materialName];

	MaterialData materialData = OBJLoader::loadMaterial(fileName, materialName);

	bool diffuseColor = materialData.vectors.count(bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_COLOR()) > 0;
	bool diffuseMap = materialData.textures.count(bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP()) > 0;
	bool diffuseLighting = diffuseMap || diffuseColor;
	bool specularLighting = shaderMaxLights > 0 && materialData.scalars.count(bRenderer::WAVEFRONT_MATERIAL_SPECULAR_EXPONENT()) > 0;
	bool cubicReflectionMap = materialData.cubeTextures.size() >= 6;
	ShaderPtr shader = loadShaderFile(shaderName.empty() ? materialName : shaderName, shaderMaxLights, variableNumberOfLights, ambientLighting, diffuseLighting, specularLighting, cubicReflectionMap);

	return createMaterial(materialName, materialData, shader);
}

MaterialPtr ObjectManager::loadObjMaterial(const std::string &fileName, const std::string &materialName, ShaderPtr shader)
{
	// log activity
	bRenderer::log("loading Material: " + materialName, bRenderer::LM_SYS);

	if (getModel(materialName))
		return _materials[materialName];

	return createMaterial(materialName, OBJLoader::loadMaterial(fileName, materialName), shader);
}

ModelPtr ObjectManager::loadObjModel(const std::string &fileName, bool flipT, bool flipZ, bool shaderFromFile, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting, PropertiesPtr properties)
{
	// log activity
	bRenderer::log("loading Model: " + fileName, bRenderer::LM_SYS);

	// get file name
	std::string name = getRawName(fileName);

	if (getModel(name))
		return _models[name];

	// create model
	ModelData modelData(fileName, flipT, flipZ);
	return createModel(name, modelData, shaderFromFile, shaderMaxLights, variableNumberOfLights, ambientLighting, properties);
}

ModelPtr ObjectManager::loadObjModel(const std::string &fileName, bool flipT, bool flipZ, ShaderPtr shader, PropertiesPtr properties)
{
	// log activity
	bRenderer::log("loading Model: " + fileName, bRenderer::LM_SYS);

	// get file name
	std::string name = getRawName(fileName);

	if (getModel(name))
		return _models[name];

	// create model
	ModelData modelData(fileName, flipT, flipZ);

	return createModel(name, modelData, shader, properties);
}

ModelPtr ObjectManager::loadObjModel(const std::string &fileName, bool flipT, bool flipZ, MaterialPtr material, PropertiesPtr properties)
{
	// log activity
	bRenderer::log("loading Model: " + fileName, bRenderer::LM_SYS);

	// get file name
	std::string name = getRawName(fileName);

	if (getModel(name))
		return _models[name];

	// create model
	ModelData modelData(fileName, flipT, flipZ);
	return createModel(name, modelData, material, properties);
}

TexturePtr ObjectManager::loadTexture(const std::string &fileName)
{
	// get file name
	std::string name = getRawName(fileName);

	if (getTexture(name))
		return _textures[name];

	// create texture data
	TextureData textureData(fileName);
	// create texture
	return createTexture(name, textureData);
}

CubeMapPtr ObjectManager::loadCubeMap(const std::string &name, const std::vector<std::string> &fileNames)
{
	if (getCubeMap(name))
		return _cubeMaps[name];

	// create texture data
	std::vector<TextureData> data;
	if (fileNames.size() >= 6){
		for (GLuint i = 0; i < 6; i++){
			data.push_back(TextureData(fileNames[i]));
		}
	}
	// create cube map
	return createCubeMap(name, data);
}

FontPtr ObjectManager::loadFont(const std::string &fileName, GLuint fontPixelSize)
{
	// get file name
	std::string name = getRawName(fileName);

	if (getFont(name)) return getFont(name);
	FontPtr &font = _fonts[name];

	font = FontPtr(new Font(fileName, fontPixelSize));

	return font;
}

ShaderPtr ObjectManager::loadShaderFile(const std::string &shaderName, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting, bool diffuseLighting, bool specularLighting, bool cubicReflectionMap)
{
	std::string name = getRawName(shaderName);

	if (getShader(name))
		return _shaders[name];

	ShaderDataFile shaderData(shaderName, _shaderVersionDesktop, _shaderVersionES, shaderMaxLights, variableNumberOfLights, ambientLighting, diffuseLighting, specularLighting, cubicReflectionMap);
	ShaderPtr shader = createShader(name, shaderData);
	if (shader) return shader;

	bRenderer::log("Couldn't load shader '" + name + "'.", bRenderer::LM_INFO);
	return nullptr;
}

ShaderPtr ObjectManager::generateShader(const std::string &shaderName, GLuint shaderMaxLights, bool ambientLighting, const MaterialData &materialData, bool variableNumberOfLights, bool isText)
{
	std::string name = getRawName(shaderName);

	if (getShader(name))
		return _shaders[name];

	ShaderDataGenerator shaderGenerator(shaderMaxLights, ambientLighting, materialData, variableNumberOfLights, isText);
	return createShader(name, shaderGenerator);
}

ShaderPtr ObjectManager::generateShader(const std::string &shaderName, const ShaderGeneratorSettings &shaderGeneratorSettings)
{
	std::string name = getRawName(shaderName);

	if (getShader(name))
		return _shaders[name];

	ShaderDataGenerator shaderGenerator(shaderGeneratorSettings);
	return createShader(name, shaderGenerator);
}

MaterialPtr ObjectManager::createMaterial(const std::string &name, ShaderPtr shader)
{
	if (getMaterial(name)) return getMaterial(name);
	MaterialPtr &material = _materials[name];

	material = MaterialPtr(new Material);
	material->setShader(shader);
	material->setName(name);
	return material;
}

MaterialPtr ObjectManager::createMaterial(const std::string &name, const MaterialData &materialData, ShaderPtr shader)
{
	if (getMaterial(name)) return getMaterial(name);
	MaterialPtr &material = _materials[name];

	material = MaterialPtr(new Material);
	material->initialize(this, materialData, shader);
	return material;
}

MaterialPtr ObjectManager::createMaterialShaderCombination(const std::string &name, const MaterialData &materialData, bool shaderFromFile, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting, bool isText)
{
	if (getMaterial(name)) return getMaterial(name);
	MaterialPtr &material = _materials[name];
	ShaderPtr shader;

	if (shaderFromFile){
		bool diffuseColor = materialData.vectors.count(bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_COLOR()) > 0;
		bool diffuseMap = materialData.textures.count(bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP()) > 0;
		bool diffuseLighting = diffuseMap || diffuseColor;
		bool specularLighting = shaderMaxLights > 0 && materialData.scalars.count(bRenderer::WAVEFRONT_MATERIAL_SPECULAR_EXPONENT()) > 0;
		bool cubicReflectionMap = materialData.cubeTextures.size() >= 6;
		shader = loadShaderFile(name, shaderMaxLights, variableNumberOfLights, ambientLighting, diffuseLighting, specularLighting, cubicReflectionMap);
	}
	else{
		shader = generateShader(name, shaderMaxLights, ambientLighting, materialData, variableNumberOfLights, isText);
	}
	material = MaterialPtr(new Material);
	material->initialize(this, materialData, shader);
	return material;
}

PropertiesPtr ObjectManager::createProperties(const std::string &name)
{
	if (getProperties(name)) return getProperties(name);
	PropertiesPtr &properties = _properties[name];

	properties = PropertiesPtr(new Properties);
	properties->setName(name);
	return properties;
}

ModelPtr ObjectManager::createModel(const std::string &name, const ModelData &modelData, bool shaderFromFile, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Model(this, modelData, shaderMaxLights, variableNumberOfLights, shaderFromFile, ambientLighting, properties));
	return model;
}

ModelPtr ObjectManager::createModel(const std::string &name, const ModelData &modelData, ShaderPtr shader, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Model(this, modelData, shader, properties));
	return model;
}

ModelPtr ObjectManager::createModel(const std::string &name, const ModelData &modelData, MaterialPtr material, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Model(modelData, material, properties));
	return model;
}

ModelPtr ObjectManager::createSprite(const std::string &name, MaterialPtr material, bool flipT, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Sprite(material, flipT, properties));
	return model;
}

ModelPtr ObjectManager::createSprite(const std::string &name, const std::string &textureFileName, ShaderPtr shader, bool flipT, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Sprite(this, textureFileName, name, shader, flipT, properties));
	return model;
}

ModelPtr ObjectManager::createSprite(const std::string &name, const std::string &textureFileName, GLuint shaderMaxLights, bool variableNumberOfLights, bool flipT, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Sprite(this, name, textureFileName, shaderMaxLights, variableNumberOfLights, flipT, properties));
	return model;
}

TextSpritePtr ObjectManager::createTextSprite(const std::string &name, vmml::Vector3f color, const std::string &text, FontPtr font, PropertiesPtr properties)
{
	if (getTextSprite(name)) return getTextSprite(name);
	TextSpritePtr &textSprite = _textSprites[name];

	textSprite = TextSpritePtr(new TextSprite(this, name, color, text, font, properties));
	return textSprite;
}

TextSpritePtr ObjectManager::createTextSprite(const std::string &name, MaterialPtr material, const std::string &text, FontPtr font, PropertiesPtr properties)
{
	if (getTextSprite(name)) return getTextSprite(name);
	TextSpritePtr &textSprite = _textSprites[name];

	textSprite = TextSpritePtr(new TextSprite(material, text, font, properties));
	return textSprite;
}

TexturePtr ObjectManager::createTexture(const std::string &name, const TextureData &textureData)
{
	if (getTexture(name)) return getTexture(name);
	TexturePtr &texture = _textures[name];

	texture = TexturePtr(new Texture(textureData));

	return texture;
}

TexturePtr ObjectManager::createTexture(const std::string &name, GLsizei width, GLsizei height, GLenum format, ImageDataPtr imageData)
{
	if (getTexture(name)) return getTexture(name);
	TexturePtr &texture = _textures[name];

	// create texture
	TextureData textureData(width, height, format, imageData);

	texture = TexturePtr(new Texture(textureData));

	return texture;
}

CubeMapPtr ObjectManager::createCubeMap(const std::string &name, const std::vector<TextureData> &data)
{
	if (getCubeMap(name)) return getCubeMap(name);
	CubeMapPtr &cubeMap = _cubeMaps[name];

	cubeMap = CubeMapPtr(new CubeMap(data));

	return cubeMap;
}

CubeMapPtr ObjectManager::createCubeMap(const std::string &name, GLsizei width, GLenum format, const std::vector<ImageDataPtr> &imageData)
{
	if (getCubeMap(name)) return getCubeMap(name);
	CubeMapPtr &cubeMap = _cubeMaps[name];

	// create empty texture data
	std::vector<TextureData> data;
	if (imageData.size() < 6){
		for (GLuint i = 0; i < 6; i++)
			data.push_back(TextureData(width, width, format, nullptr));
	}
	else{
		for (GLuint i = 0; i < 6; i++)
			data.push_back(TextureData(width, width, format, imageData.at(i)));
	}

	cubeMap = CubeMapPtr(new CubeMap(data));

	return cubeMap;
}

DepthMapPtr ObjectManager::createDepthMap(const std::string &name, GLint width, GLint height)
{
	if (getDepthMap(name)) return getDepthMap(name);
	DepthMapPtr &depthMap = _depthMaps[name];

	depthMap = DepthMapPtr(new DepthMap(width, height));

	return depthMap;
}

ShaderPtr ObjectManager::createShader(const std::string &name, const IShaderData &shaderData)
{
	if (shaderData.isValid())
	{
		if (getShader(name)) return getShader(name);
		ShaderPtr &shader = _shaders[name];

		bRenderer::log("Created shader '" + name + "'.", bRenderer::LM_INFO);
		shader = ShaderPtr(new Shader(shaderData));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_POSITION(), 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_NORMAL(), 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_TANGENT(), 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tangent));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_BITANGENT(), 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, bitangent));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_TEXCOORD(), 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
		return shader;
	}

	return nullptr;
}

CameraPtr ObjectManager::createCamera(const std::string &name)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera());
	return camera;
}

CameraPtr ObjectManager::createCamera(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera(position, rotationAxes));
	return camera;
}

CameraPtr ObjectManager::createCamera(const std::string &name, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera(fov, aspect, near, far));
	return camera;
}

CameraPtr ObjectManager::createCamera(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera(position, rotationAxes, fov, aspect, near, far));
	return camera;
}

MatrixStackPtr ObjectManager::createMatrixStack(const std::string &name)
{
	if (getMatrixStack(name)) return getMatrixStack(name);
	MatrixStackPtr &matrixStack = _matrixStacks[name];

	matrixStack = MatrixStackPtr(new MatrixStack());
	return matrixStack;
}

LightPtr ObjectManager::createLight(const std::string &name)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light());
	return light;
}

LightPtr ObjectManager::createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &color)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light(position, color));
	return light;
}

LightPtr ObjectManager::createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &color, GLfloat intensity, GLfloat attenuation, GLfloat radius)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light(position, color, intensity, attenuation, radius));
	return light;
}

LightPtr ObjectManager::createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &diffuseColor, const vmml::Vector3f &specularColor, GLfloat intensity, GLfloat attenuation, GLfloat radius)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light(position, diffuseColor, specularColor, intensity, attenuation, radius));
	return light;
}

FramebufferPtr ObjectManager::createFramebuffer(const std::string &name)
{
	if (getFramebuffer(name)) return getFramebuffer(name);
	FramebufferPtr &framebuffer = _framebuffers[name];
	framebuffer = FramebufferPtr(new Framebuffer());
	return framebuffer;
}

FramebufferPtr ObjectManager::createFramebuffer(const std::string &name, GLint width, GLint height)
{
    if (getFramebuffer(name)) return getFramebuffer(name);
    FramebufferPtr &framebuffer = _framebuffers[name];
    framebuffer = FramebufferPtr(new Framebuffer(width, height));
    return framebuffer;
}

bool ObjectManager::addShader(const std::string &name, ShaderPtr ptr)
{
	if (getShader(name)) return false;
	_shaders.insert(ShaderMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addTexture(const std::string &name, TexturePtr ptr)
{
	if (getTexture(name)) return false;
	_textures.insert(TextureMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addCubeMap(const std::string &name, CubeMapPtr ptr)
{
	if (getCubeMap(name)) return false;
	_cubeMaps.insert(CubeMapMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addDepthMap(const std::string &name, DepthMapPtr ptr)
{
	if (getDepthMap(name)) return false;
	_depthMaps.insert(DepthMapMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addFont(const std::string &name, FontPtr ptr)
{
	if (getFont(name)) return false;
	_fonts.insert(FontMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addMaterial(const std::string &name, MaterialPtr ptr)
{
	if (getMaterial(name)) return false;
	_materials.insert(MaterialMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addProperties(const std::string &name, PropertiesPtr ptr)
{
	if (getProperties(name)) return false;
	_properties.insert(PropertiesMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addModel(const std::string &name, ModelPtr ptr)
{
	if (getModel(name)) return false;
	_models.insert(ModelMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addTextSprite(const std::string &name, TextSpritePtr ptr)
{
	if (getTextSprite(name)) return false;
	_textSprites.insert(TextSpriteMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addCamera(const std::string &name, CameraPtr ptr)
{
	if (getCamera(name)) return false;
	_cameras.insert(CameraMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addMatrixStack(const std::string &name, MatrixStackPtr ptr)
{
	if (getMatrixStack(name)) return false;
	_matrixStacks.insert(MatrixStackMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addLight(const std::string &name, LightPtr ptr)
{
	if (getLight(name)) return false;
	_lights.insert(LightMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addFramebuffer(const std::string &name, FramebufferPtr ptr)
{
	if (getFramebuffer(name)) return false;
	_framebuffers.insert(FramebufferMap::value_type(name, ptr));
	return true;
}

bool ObjectManager::addDrawable(const std::string &name, DrawablePtr ptr)
{
	if (getDrawable(name)) return false;
	_drawables.insert(DrawableMap::value_type(name, ptr));
	return true;
}

ShaderPtr ObjectManager::getShader(const std::string &name)
{
	if (_shaders.count(name) > 0)
		return _shaders[name];
	return nullptr;
}

TexturePtr ObjectManager::getTexture(const std::string &name)
{
	if (_textures.count(name) > 0)
		return _textures[name];
	return nullptr;
}

CubeMapPtr ObjectManager::getCubeMap(const std::string &name)
{
	if (_cubeMaps.count(name) > 0)
		return _cubeMaps[name];
	return nullptr;
}

DepthMapPtr ObjectManager::getDepthMap(const std::string &name)
{
	if (_depthMaps.count(name) > 0)
		return _depthMaps[name];
	return nullptr;
}

FontPtr ObjectManager::getFont(const std::string &name)
{
	if (_fonts.count(name) > 0)
		return _fonts[name];
	return nullptr;
}

MaterialPtr ObjectManager::getMaterial(const std::string &name)
{
	if (_materials.count(name) > 0)
		return _materials[name];
	return nullptr;
}

PropertiesPtr ObjectManager::getProperties(const std::string &name)
{
	if (_properties.count(name) > 0)
		return _properties[name];
	return nullptr;
}

ModelPtr ObjectManager::getModel(const std::string &name)
{
	if (_models.count(name) > 0)
		return _models[name];
	return nullptr;
}

TextSpritePtr ObjectManager::getTextSprite(const std::string &name)
{
	if (_textSprites.count(name) > 0)
		return _textSprites[name];
	return nullptr;
}

CameraPtr ObjectManager::getCamera(const std::string &name)
{
	if (_cameras.count(name) > 0)
		return _cameras[name];
	return nullptr;
}

MatrixStackPtr ObjectManager::getMatrixStack(const std::string &name)
{
	if (_matrixStacks.count(name) > 0)
		return _matrixStacks[name];
	return nullptr;
}

LightPtr ObjectManager::getLight(const std::string &name)
{
	if (_lights.count(name) > 0)
		return _lights[name];
	return nullptr;
}

FramebufferPtr ObjectManager::getFramebuffer(const std::string &name)
{
	if (_framebuffers.count(name) > 0)
		return _framebuffers[name];
	return nullptr;
}

DrawablePtr ObjectManager::getDrawable(const std::string &name)
{
	if (_drawables.count(name) > 0)
		return _drawables[name];
	return nullptr;
}

std::string ObjectManager::getShaderVersionDesktop()
{
	return _shaderVersionDesktop;
}

std::string ObjectManager::getShaderVersionES()
{
	return _shaderVersionES;
}

vmml::Vector3f ObjectManager::getAmbientColor()
{
	return _ambientColor;
}

void ObjectManager::removeShader(const std::string &name, bool del)
{
	if (del && _shaders.count(name) > 0){
		_shaders[name]->deleteShader();
	}
	_shaders.erase(name);
}

void ObjectManager::removeTexture(const std::string &name, bool del)
{
	if (del && _textures.count(name) > 0){
		_textures[name]->deleteTexture();
	}
	_textures.erase(name);
}

void ObjectManager::removeCubeMap(const std::string &name, bool del)
{
	if (del && _cubeMaps.count(name) > 0){
		_cubeMaps[name]->deleteTexture();
	}
	_cubeMaps.erase(name);
}

void ObjectManager::removeDepthMap(const std::string &name, bool del)
{
	if (del && _depthMaps.count(name) > 0){
		_depthMaps[name]->deleteTexture();
	}
	_depthMaps.erase(name);
}

void ObjectManager::removeFont(const std::string &name, bool del)
{
	if (del && _fonts.count(name) > 0){
		_fonts[name]->deleteFont();
	}
	_fonts.erase(name);
}

void ObjectManager::removeMaterial(const std::string &name)
{
	_materials.erase(name);
}

void ObjectManager::removeProperties(const std::string &name)
{
	_properties.erase(name);
}

void ObjectManager::removeModel(const std::string &name, bool del)
{
	if (del && _models.count(name) > 0){
		_models[name]->deleteModelGeometry();
	}
	_models.erase(name);
}

void ObjectManager::removeTextSprite(const std::string &name, bool del)
{
	if (del && _textSprites.count(name) > 0){
		_textSprites[name]->deleteModelGeometry();
	}
	_textSprites.erase(name);
}

void ObjectManager::removeCamera(const std::string &name)
{
	_cameras.erase(name);
}

void ObjectManager::removeMatrixStack(const std::string &name)
{
	_matrixStacks.erase(name);
}

void ObjectManager::removeLight(const std::string &name)
{
	_lights.erase(name);
}

void ObjectManager::removeFramebuffer(const std::string &name, bool del)
{
	if (del && _framebuffers.count(name) > 0){
		_framebuffers[name]->deleteFramebuffer();
	}
	_framebuffers.erase(name);
}

void ObjectManager::removeDrawable(const std::string &name)
{
	_drawables.erase(name);
}

void ObjectManager::clear(bool del)
{
	if(del) for (auto i = _shaders.begin(); i != _shaders.end(); ++i)
	{
		i->second->deleteShader();
	}
	_shaders.clear();

	if (del) for (auto i = _textures.begin(); i != _textures.end(); ++i)
	{
		i->second->deleteTexture();
	}
	_textures.clear();

	if (del) for (auto i = _cubeMaps.begin(); i != _cubeMaps.end(); ++i)
	{
		i->second->deleteTexture();
	}
	_cubeMaps.clear();

	if (del) for (auto i = _depthMaps.begin(); i != _depthMaps.end(); ++i)
	{
		i->second->deleteTexture();
	}
	_depthMaps.clear();

	if (del) for (auto i = _fonts.begin(); i != _fonts.end(); ++i)
	{
		i->second->deleteFont();
	}
	_fonts.clear();

	_materials.clear();
	
	_properties.clear();
	
	if (del) for (auto i = _models.begin(); i != _models.end(); ++i)
	{
		i->second->deleteModelGeometry();
	}
	_models.clear();

	if (del) for (auto i = _textSprites.begin(); i != _textSprites.end(); ++i)
	{
		i->second->deleteModelGeometry();
	}
	_textSprites.clear();

	_cameras.clear();
	_matrixStacks.clear();
	_lights.clear();
	
	if (del) for (auto i = _framebuffers.begin(); i != _framebuffers.end(); ++i)
	{
		i->second->deleteFramebuffer();
	}
	_framebuffers.clear();
	
	_drawables.clear();

	_ambientColor = bRenderer::DEFAULT_AMBIENT_COLOR();

	_shaderVersionDesktop = bRenderer::DEFAULT_SHADER_VERSION_DESKTOP();
	_shaderVersionES = bRenderer::DEFAULT_SHADER_VERSION_ES();
}

/* Private functions */

std::string ObjectManager::getRawName(const std::string &fileName, std::string *ext)
{
	std::string rawName = fileName;
	int indexSlash = rawName.find_last_of("/\\");

	if (indexSlash != std::string::npos)
	{
		rawName = rawName.substr(indexSlash + 1);
	}

	int indexDot = rawName.find_last_of(".");
	if (indexDot != std::string::npos)
	{
		if (ext) *ext = rawName.substr(indexDot + 1, std::string::npos);
		return rawName.substr(0, indexDot);
	}
	return rawName;
}
