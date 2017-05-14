#include "headers/Geometry.h"

/* Public functions */

void Geometry::initialize(GeometryDataPtr geometryData)
{
	// Create bounding volume
	_boundingBox = createBoundingBoxObjectSpace(geometryData->vboVertices);
	
	// Initialize geometry
    copyVertexData(geometryData->vboVertices);
    copyIndexData(geometryData->vboIndices);
    initializeVertexBuffer();

	_initialized = true;
}

void Geometry::draw(GLenum mode)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

	if (_material)
		_material->bind();

	if (_properties)
		_properties->passToShader(_material->getShader());

	glDrawElements(mode, _nIndices, GL_UNSIGNED_SHORT, _indexData.get());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Geometry::drawInstance(const std::string &instanceName, GLenum mode)
{
	if (getInstanceProperties(instanceName)){
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

		if (_material)
			_material->bind();

		if (_properties)
			_properties->passToShader(_material->getShader());

		// Instance properties may override properties set for the geometry in  general
		getInstanceProperties(instanceName)->passToShader(_material->getShader());

		glDrawElements(mode, _nIndices, GL_UNSIGNED_SHORT, _indexData.get());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}


PropertiesPtr Geometry::addInstance(const std::string &instanceName)
{
	if (getInstanceProperties(instanceName)) return getInstanceProperties(instanceName); // return if already existing
	PropertiesPtr &properties = _instances[instanceName];
	properties = PropertiesPtr(new Properties);
	properties->setName(instanceName);
	return properties;
}

void Geometry::addInstance(const std::string &instanceName, PropertiesPtr instanceProperties)
{
	_instances.insert(PropertiesMap::value_type(instanceName, instanceProperties));
}

PropertiesPtr Geometry::getInstanceProperties(const std::string &instanceName)
{
	if (_instances.count(instanceName) > 0)
		return _instances[instanceName];
	return nullptr;
}
void Geometry::removeInstance(const std::string &instanceName)
{
	_instances.erase(instanceName);
}

void Geometry::clearInstances()
{
	_instances.clear();
}

/* Private functions */

Geometry::VertexDataPtr Geometry::allocVertexData(size_t nVertices)
{
    _nVertices  = nVertices;
    _vertexData = VertexDataPtr(new Vertex[_nVertices]);
    
    return _vertexData;
}

Geometry::IndexDataPtr Geometry::allocIndexData(size_t nIndices)
{
    _nIndices  = nIndices;
    _indexData = IndexDataPtr(new GLushort[_nIndices]);
    
    return _indexData;
}

Geometry::VertexDataPtr Geometry::copyVertexData(const GeometryData::VboVertices &arg)
{
	if (arg.size() > 0){
		allocVertexData(arg.size());
		memcpy(_vertexData.get(), &arg[0], sizeof(Vertex) * _nVertices);
	}
    return _vertexData;
}

Geometry::IndexDataPtr Geometry::copyIndexData(const GeometryData::VboIndices &arg)
{
	if (arg.size() > 0){
		allocIndexData(arg.size());
		memcpy(_indexData.get(), &arg[0], sizeof(Index) * _nIndices);
	}
    return _indexData;
}

void Geometry::initializeVertexBuffer()
{
	if (!_initialized)
		glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _nVertices*sizeof(Vertex), _vertexData.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

vmml::AABBf Geometry::createBoundingBoxObjectSpace(const GeometryData::VboVertices &arg)
{
	Point3 min = arg[0].position;
	Point3 max = arg[0].position;

	for (auto i = arg.begin(); i != arg.end(); ++i)
	{
		if (i->position.x < min.x)
			min.x = i->position.x;
		else if (i->position.x > max.x)
			max.x = i->position.x;

		if (i->position.y < min.y)
			min.y = i->position.y;
		else if (i->position.y > max.y)
			max.y = i->position.y;

		if (i->position.z < min.z)
			min.z = i->position.z;
		else if (i->position.z > max.z)
			max.z = i->position.z;
	}

	return vmml::AABBf(vmml::Vector3f(min.x, min.y, min.z), vmml::Vector3f(max.x, max.y, max.z));
}

