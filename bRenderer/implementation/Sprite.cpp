#include "headers/Sprite.h"
#include "headers/Configuration.h"
#include "headers/ObjectManager.h"

Sprite::Sprite(MaterialPtr material, bool flipT, PropertiesPtr	properties)
{
	createGeometry(flipT);

	setMaterial(material);
	setProperties(properties);
}

Sprite::Sprite(ObjectManager *o, const std::string &textureFileName, const std::string &materialName, ShaderPtr shader, bool flipT, PropertiesPtr	properties)
{
	createGeometry(flipT);

	MaterialData md; 
	md.textures[bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP()] = textureFileName;

	MaterialPtr material = o->createMaterial(materialName, md, shader);

	setMaterial(material);
	setProperties(properties);
}

Sprite::Sprite(ObjectManager *o, const std::string &name, const std::string &textureFileName, GLuint shaderMaxLights, bool variableNumberOfLights, bool flipT, PropertiesPtr	properties)
{
	createGeometry(flipT);

	MaterialData md;
	md.textures[bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP()] = textureFileName;

	ShaderPtr shader = o->generateShader(name, shaderMaxLights, false, md, variableNumberOfLights, false);

	MaterialPtr material = o->createMaterial(name, md, shader);

	setMaterial(material);
	setProperties(properties);
}

void Sprite::createGeometry(bool flipT)
{
	Model::GroupMap &groups = getGroups();
	GeometryPtr g = GeometryPtr(new Geometry);
	groups.insert(std::pair< std::string, GeometryPtr >(bRenderer::DEFAULT_GROUP_NAME(), g));

	GeometryDataPtr gData = GeometryDataPtr(new GeometryData);
	// Add vertices
	gData->vboVertices.push_back(Vertex(
		1.0f, 1.0f, 0.0f,			// position
		0.0f, 0.0f, -1.0f,			// normal
		-1.0f, 0.0f, 0.0f,			// tangent
		0.0f, 1.0f, 0.0f,			// bitangent
		1.0f, flipT ? 0.0f : 1.0f	// texCoord	
	));
	gData->vboVertices.push_back(Vertex(
		-1.0f, 1.0f, 0.0f, 			// position
		0.0f, 0.0f, -1.0f,			// normal
		-1.0f, 0.0f, 0.0f,			// tangent
		0.0f, 1.0f, 0.0f,			// bitangent
		0.0f, flipT ? 0.0f : 1.0f	// texCoord
	));
	gData->vboVertices.push_back(Vertex(
		-1.0f, -1.0f, 0.0f,			// position
		0.0f, 0.0f, -1.0f,			// normal
		-1.0f, 0.0f, 0.0f,			// tangent
		0.0f, 1.0f, 0.0f,			// bitangent
		0.0f, flipT ? 1.0f : 0.0f	// texCoord
	));
	gData->vboVertices.push_back(Vertex(
		1.0f, -1.0f, 0.0f,			// position
		0.0f, 0.0f, -1.0f,			// normal
		-1.0f, 0.0f, 0.0f,			// tangent
		0.0f, 1.0f, 0.0f,			// bitangent
		1.0f, flipT ? 1.0f : 0.0f	// texCoord
	));
	gData->vboVertices.push_back(Vertex(
		1.0f, 1.0f, 0.0f,			// position
		0.0f, 0.0f, -1.0f,			// normal
		-1.0f, 0.0f, 0.0f,			// tangent
		0.0f, 1.0f, 0.0f,			// bitangent
		1.0f, flipT ? 0.0f : 1.0f	// texCoord
	));
	gData->vboVertices.push_back(Vertex(
		-1.0f, -1.0f, 0.0f,			// position
		0.0f, 0.0f, -1.0f,			// normal
		-1.0f, 0.0f, 0.0f,			// tangent
		0.0f, 1.0f, 0.0f,			// bitangent
		0.0f, flipT ? 1.0f : 0.0f	// texCoord
	));

	// Add indices
	for (GLushort i = 0; i < 6; i++)
		gData->vboIndices.push_back(i);

	// Add index data
	gData->indices.push_back(IndexData(3, 2, 0));
	gData->indices.push_back(IndexData(2, 1, 0));
	gData->indices.push_back(IndexData(0, 0, 0));
	gData->indices.push_back(IndexData(1, 5, 0));
	gData->indices.push_back(IndexData(3, 4, 0));
	gData->indices.push_back(IndexData(0, 3, 0));

	g->initialize(gData);

	setBoundingBoxObjectSpace(g->getBoundingBoxObjectSpace());
}