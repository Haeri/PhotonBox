#include "headers/Model.h"
#include "headers/ModelData.h"
#include "headers/TextureData.h"
#include "headers/ObjectManager.h"

Model::Model(ObjectManager *o, const ModelData &modelData, GLuint shaderMaxLights, bool variableNumberOfLights, bool shaderFromFile, bool ambientLighting, PropertiesPtr properties)
	: _properties(properties)
{
	ModelData::GroupMap data = modelData.getData();

	for (auto i = data.begin(); i != data.end(); ++i)
	{
		GeometryPtr g = GeometryPtr(new Geometry);
		_groups.insert(std::pair< std::string, GeometryPtr >(i->first, g));
		GeometryDataPtr gData = i->second;

		MaterialPtr material = o->createMaterialShaderCombination(gData->materialData.name, gData->materialData, shaderFromFile, shaderMaxLights, variableNumberOfLights, ambientLighting);
		g->initialize(gData);
		g->setMaterial(material);
		g->setProperties(properties);

		// expand bounding box
		_boundingBox.merge(g->getBoundingBoxObjectSpace());
	}
}

Model::Model(ObjectManager *o, const ModelData &modelData, ShaderPtr shader, PropertiesPtr properties)
	: _properties(properties)
{
    ModelData::GroupMap data = modelData.getData();
    for (auto i = data.begin(); i != data.end(); ++i)
    {
		GeometryPtr g = GeometryPtr(new Geometry);
		_groups.insert(std::pair< std::string, GeometryPtr >(i->first, g));
        GeometryDataPtr gData = i->second;
		MaterialPtr material = o->createMaterial(gData->materialData.name, gData->materialData, shader);
        g->initialize(gData);
        g->setMaterial(material);
		g->setProperties(properties);

		// expand bounding box
		_boundingBox.merge(g->getBoundingBoxObjectSpace());
    }
}

Model::Model(const ModelData &modelData, MaterialPtr material, PropertiesPtr properties)
{
	ModelData::GroupMap data = modelData.getData();
	_material = material;
	_properties = properties;
	for (auto i = data.begin(); i != data.end(); ++i)
	{
		GeometryPtr g = GeometryPtr(new Geometry);
		_groups.insert(std::pair< std::string, GeometryPtr >(i->first, g));
		GeometryDataPtr gData = i->second;
		g->initialize(gData);
		g->setMaterial(material);
		g->setProperties(properties);

		// expand bounding box
		_boundingBox.merge(g->getBoundingBoxObjectSpace());
	}
}

void Model::draw(GLenum mode)
{
    for (auto i = _groups.begin(); i != _groups.end(); ++i)
    {
        i->second->draw(mode);
    }
}

void Model::draw(const std::string &geometryName, GLenum mode)
{
	_groups[geometryName]->draw(mode);
}

void Model::drawInstance(const std::string &instanceName, GLenum mode)
{
	for (auto i = _groups.begin(); i != _groups.end(); ++i)
	{
		i->second->drawInstance(instanceName, mode);
	}
}

Model::InstanceMapPtr Model::addInstance(const std::string &instanceName)
{
	// add instance in model
	if (getInstanceProperties(instanceName)) return getInstanceProperties(instanceName); // return if already existing
	InstanceMapPtr &instanceMap = _instances[instanceName];
	instanceMap = InstanceMapPtr(new std::unordered_map< ShaderPtr, PropertiesPtr >);
	
	// add in geometry
	for (auto i = _groups.begin(); i != _groups.end(); ++i)
	{
		ShaderPtr shader = i->second->getMaterial()->getShader();
		// If properties exist for this shader they are added to the geometry instance
		if (instanceMap->count(shader) > 0)
			i->second->addInstance(instanceName, instanceMap->at(shader));
		// Otherwise we create new properties
		else{
			PropertiesPtr properties = i->second->addInstance(instanceName);
			instanceMap->insert(std::unordered_map< ShaderPtr, PropertiesPtr >::value_type(shader, properties));
		}
	}

	return instanceMap;
}

PropertiesPtr Model::getInstanceProperties(const std::string &instanceName, const std::string &geometryName)
{
	return _groups[geometryName]->getInstanceProperties(instanceName);
}

Model::InstanceMapPtr Model::getInstanceProperties(const std::string &instanceName)
{
	if (_instances.count(instanceName) > 0)
		return _instances[instanceName];
	return nullptr;
}

void Model::removeInstance(const std::string &instanceName)
{
	// remove from model
	_instances.erase(instanceName);
	// remove from associated geometry
	for (auto i = _groups.begin(); i != _groups.end(); ++i)
	{
		i->second->removeInstance(instanceName);
	}
}

void Model::clearInstances()
{
	// clear in model
	_instances.clear();
	// clear in associated geometry
	for (auto i = _groups.begin(); i != _groups.end(); ++i)
	{
		i->second->clearInstances();
	}
}

void  Model::addGeometry(const std::string &name, GeometryPtr geometry)
{ 
	// add geometry
	_groups.insert(GroupMap::value_type(name, geometry)); 
	
	// add instances
	ShaderPtr shader = geometry->getMaterial()->getShader();
	for (auto i = _instances.begin(); i != _instances.end(); ++i)
	{
		std::string instanceName = i->first;
		InstanceMapPtr instanceMap = i->second;

		// If properties exist for this shader they are added to the geometry instance
		if (instanceMap->count(shader) > 0)
			geometry->addInstance(instanceName, instanceMap->at(shader));
		// Otherwise we create new properties
		else{
			PropertiesPtr properties = geometry->addInstance(instanceName);
			instanceMap->insert(std::unordered_map< ShaderPtr, PropertiesPtr >::value_type(shader, properties));
		}
	}

	// expand bounding box
	_boundingBox.merge(geometry->getBoundingBoxObjectSpace());
}