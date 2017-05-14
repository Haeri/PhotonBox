#ifndef B_MODEL_H
#define B_MODEL_H

#include <memory>
#include <string>
#include <unordered_map>

#include "IDrawable.h"
#include "Geometry.h"
#include "Material.h"
#include "Renderer_GL.h"

class ObjectManager;
class ModelData;

/** @brief A 3d model that can be rendered to the screen
*	@author Rahul Mukhi, David Steiner
*/
class Model : public IDrawable
{
public:
	/* Typedefs */
	typedef std::unordered_map< std::string, GeometryPtr > GroupMap;
	typedef  std::shared_ptr< std::unordered_map< ShaderPtr, PropertiesPtr > >	InstanceMapPtr;
	typedef std::unordered_map< std::string, InstanceMapPtr >	InstancesMap;

	/* Functions */

	/**	@brief Constructor
	*/
	Model() {}

	/**	@brief Constructor
	*	@param[in] o Object management
	*	@param[in] modelData
	*	@param[in] shaderMaxLights The maximum light sources to be used
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	Model(ObjectManager *o, const ModelData &modelData, GLuint shaderMaxLights, bool variableNumberOfLights, bool shaderFromFile, bool ambientLighting, PropertiesPtr properties = nullptr);

	/**	@brief Constructor
	*	@param[in] o Object management
	*	@param[in] modelData
	*	@param[in] shader
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	Model(ObjectManager *o, const ModelData &modelData, ShaderPtr shader, PropertiesPtr	properties = nullptr);

	/**	@brief Constructor
	*	@param[in] modelData
	*	@param[in] material 
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	Model(const ModelData &modelData, MaterialPtr material, PropertiesPtr	properties = nullptr);

	/**	@brief Virtual destructor
	*/
	virtual ~Model() { deleteModelGeometry(); }
    
	/**	@brief Draws the model to the screen
	*	@param[in] mode
	*/
	virtual void draw(GLenum mode = GL_TRIANGLES) override;

	/**	@brief Draws the specified group of geometry to the screen
	*	@param[in] geometryName Name of the group to draw
	*	@param[in] mode
	*/
	virtual void draw(const std::string &geometryName, GLenum mode = GL_TRIANGLES);

	/**	@brief Draws an instance of the model to the screen
	*	@param[in] instanceName
	*	@param[in] mode
	*/
	virtual void drawInstance(const std::string &instanceName, GLenum mode = GL_TRIANGLES) override;

	/**	@brief Creates an instance of this model and associated geometry
	*
	*	All geometry that belong to this model get additional properties
	*
	*	@param[in] instanceName	Name of the instance
	*/
	virtual InstanceMapPtr	addInstance(const std::string &instanceName);

	/**	@brief Get the properties of a geometry instance
	*	@param[in] instanceName	Name of the instance
	*	@param[in] geometryName	Name of the geometry
	*/
	virtual PropertiesPtr	getInstanceProperties(const std::string &instanceName, const std::string &geometryName);

	/**	@brief Get the instance properties for every shader used in the model
	*	@param[in] instanceName	Name of the instance
	*/
	virtual InstanceMapPtr	getInstanceProperties(const std::string &instanceName);

	/**	@brief Removes an instance of the model and associated geometry
	*	@param[in] instanceName	Name of the instance
	*/
	virtual void			removeInstance(const std::string &instanceName);

	/**	@brief Removes all instances of the model and associated geometry
	*/
	virtual void			clearInstances();
    
	/**	@brief Returns the material of the model
	*/
    MaterialPtr getMaterial()   { return _material; }
    
	/**	@brief Sets the material of the model
	*	@param[in] arg The material for the model
	*/
    void setMaterial(MaterialPtr arg)
    {
        _material = arg;
        
        for (auto i = _groups.begin(); i != _groups.end(); ++i)
        {
            i->second->setMaterial(_material);
        }
    }

	/**	@brief Returns the properties of the model
	*/
	PropertiesPtr getProperties()   { return _properties; }

	/**	@brief Sets the properties of the model
	*	@param[in] arg The properties for the model
	*/
	void setProperties(PropertiesPtr arg)
	{
		_properties = arg;

		for (auto i = _groups.begin(); i != _groups.end(); ++i)
		{
			i->second->setProperties(_properties);
		}
	}
    
	/**	@brief Returns the groups of geometry of the model
	*/
    GroupMap &getGroups()   { return _groups; }

	/**	@brief Returns the bounding box of the geometry in object space
	*/
	vmml::AABBf     &getBoundingBoxObjectSpace()			{ return _boundingBox; }

	/**	@brief Sets the bounding box of the geometry in object space
	*	@param[in] arg The bounding box for the geometry in object space
	*/
	void            setBoundingBoxObjectSpace(vmml::AABBf arg)						{ _boundingBox = arg; }

	/**	@brief Adds geometry to the model
	*	@param[in] name	Name of the geometry
	*	@param[in] geometry A pointer to the geometry
	*/
	virtual void			addGeometry(const std::string &name, GeometryPtr geometry);

	/**	@brief Removes geometry from the model
	*	@param[in] name	Name of the geometry
	*/
	virtual void			removeGeometry(const std::string &name)								{ _groups.erase(name); }

	/**	@brief Deletes all geometry of the model
	*/
	virtual void deleteModelGeometry()
	{
		for (auto i = _groups.begin(); i != _groups.end(); ++i)
		{
			i->second->deleteGeometry();
		}
		_groups.clear();
	}

private:

	/* Variables */

    GroupMap        _groups;
	MaterialPtr     _material = nullptr;
	PropertiesPtr	_properties = nullptr;
	vmml::AABBf		_boundingBox;
	InstancesMap	_instances;
};

typedef std::shared_ptr<Model> ModelPtr;

#endif /* defined(B_MODEL_H) */
