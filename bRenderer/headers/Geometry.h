#ifndef B_GEOMETRY_H
#define B_GEOMETRY_H

#include <string>
#include <fstream>
#include <memory>
#include "GeometryData.h"
#include "Material.h"
#include "Texture.h"
#include "Properties.h"
#include "Renderer_GL.h"
#include "IDrawable.h"
#include "Geometry.h"
#include "vmmlib/aabb.hpp"

/** @brief The geometry that can be rendered to the screen
*	@author Rahul Mukhi, David Steiner
*/
class Geometry : public IDrawable
{
public:
	/* Typedefs */
	typedef std::shared_ptr< Vertex >   VertexDataPtr;
    typedef std::shared_ptr< GLushort > IndexDataPtr;
	typedef std::unordered_map< std::string, PropertiesPtr >	PropertiesMap;

	/* Functions */

	/**	@brief Constructor
	*/
	Geometry(){ _initialized = false; }

	/**	@brief Virtual destructor
	*/
	virtual ~Geometry() {
        deleteGeometry();
    }

	/**	@brief Initializes the geometry object based on geometry data
	*	@param[in] geometryData
	*/
	virtual void initialize(GeometryDataPtr geometryData);

	/**	@brief Draws the geometry to the screen
	*	@param[in] mode 
	*/
	virtual void draw(GLenum mode = GL_TRIANGLES) override;

	/**	@brief Draws an instance of the geometry to the screen
	*	@param[in] instanceName
	*	@param[in] mode
	*/
	virtual void drawInstance(const std::string &instanceName, GLenum mode = GL_TRIANGLES) override;

	/**	@brief Creates an instance of this geometry
	*	@param[in] instanceName	Name of the instance
	*/
	PropertiesPtr	addInstance(const std::string &instanceName);

	/**	@brief Creates an instance of this geometry
	*	@param[in] instanceName	Name of the instance
	*	@param[in] instanceProperties Properties for the instance
	*/
	virtual void addInstance(const std::string &instanceName, PropertiesPtr instanceProperties);

	/**	@brief Get the properties of a geometry instance
	*	@param[in] instanceName	Name of the instance
	*/
	virtual PropertiesPtr	getInstanceProperties(const std::string &instanceName);

	/**	@brief Removes an instance
	*	@param[in] instanceName	Name of the instance
	*/
	virtual void			removeInstance(const std::string &instanceName);

	/**	@brief Removes all instances
	*/
	virtual void			clearInstances();

	/**	@brief Returns a pointer to the vertices of the geometry
	*/
    VertexDataPtr   getVertexData()					{ return _vertexData;   }

	/**	@brief Returns a pointer to the indices of the geometry
	*/
	IndexDataPtr    getIndexData()					{ return _indexData; }

	/**	@brief Returns the number of vertices in the geometry
	*/
	size_t			getNumVertices()				{ return _nVertices; }

	/**	@brief Returns the number of indices in the geometry
	*/
	size_t			getNumIndices()					{ return _nIndices; }
    
	/**	@brief Sets the vertices of the geometry
	*	@param[in] arg The new vertices that should be used for the geometry
	*/
    void            setVertexData(VertexDataPtr arg)    { _vertexData   = arg;  }

	/**	@brief Sets the indices of the geometry
	*	@param[in] arg The new indices that should be used for the geometry
	*/
    void            setIndexData(IndexDataPtr arg)      { _indexData    = arg;  }
    
	/**	@brief Returns a pointer to the material of the geometry
	*/
	MaterialPtr     getMaterial()					{ return _material; }

	/**	@brief Sets the material of the geometry
	*	@param[in] arg The new material that should be used for the geometry
	*/
    void            setMaterial(MaterialPtr arg)        { _material     = arg;  }

	/**	@brief Returns a pointer to the properties of the geometry
	*/
	PropertiesPtr     getProperties()					{ return _properties; }

	/**	@brief Sets the properties of the geometry
	*	@param[in] arg The properties for the geometry
	*/
	void            setProperties(PropertiesPtr arg)	{ _properties = arg; }

	/**	@brief Returns the bounding box of the geometry in object space
	*/
	vmml::AABBf     &getBoundingBoxObjectSpace()					{ return _boundingBox; }

	/**	@brief Sets the bounding box of the geometry in object space
	*	@param[in] arg The bounding box for the geometry in object space
	*/
	void            setBoundingBoxObjectSpace(vmml::AABBf arg)						{ _boundingBox = arg; }

	/**	@brief Deletes the geometry
	*/
	virtual void deleteGeometry()
	{
		if (_vertexBuffer)
			glDeleteBuffers(1, &_vertexBuffer);
		_initialized = false;
	}

protected:

	/* Functions */

	/**	@brief Initializes a vertex buffer for the geometry data
	*/
	virtual void initializeVertexBuffer();

	/**	@brief Allocates the vertex data
	*/
	virtual VertexDataPtr   allocVertexData(size_t nVertices);

	/**	@brief Allocates the index data
	*/
	virtual IndexDataPtr    allocIndexData(size_t nIndices);

	/**	@brief Copies the given vertices into the geometry object
	*	@param[in] arg The vertices that should be used in the geometry 
	*/
	virtual VertexDataPtr   copyVertexData(const GeometryData::VboVertices &arg);

	/**	@brief Copies the given indices into the geometry object
	*	@param[in] arg The indices that should be used in the geometry
	*/
	virtual IndexDataPtr    copyIndexData(const GeometryData::VboIndices &arg);

	/**	@brief Creates an axis-aligned bounding box around the object
	*	@param[in] arg The vertices that should be used in the geometry
	*/
	virtual vmml::AABBf createBoundingBoxObjectSpace(const GeometryData::VboVertices &arg);

private:

	/* Variables */

	bool _initialized = false;
    GLuint _indexBuffer = 0, _vertexBuffer = 0;
    size_t _nIndices = 0, _nVertices = 0;
    
	VertexDataPtr   _vertexData = nullptr;
	IndexDataPtr    _indexData = nullptr;
    
	MaterialPtr _material = nullptr;
	PropertiesPtr _properties = nullptr;

	vmml::AABBf _boundingBox;

	PropertiesMap _instances;
};

typedef std::shared_ptr<Geometry> GeometryPtr;

#endif /* defined(B_GEOMETRY_H) */
