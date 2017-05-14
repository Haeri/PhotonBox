#ifndef B_MATERIAL_H
#define B_MATERIAL_H

#include <memory>

#include "Shader.h"
#include "Texture.h"
#include "MaterialData.h"

class ObjectManager;

/** @brief A material associated with textures and a shader to define the look of an object
*	@author David Steiner
*/
class Material
{
public:
	/* Typedefs */
	typedef std::unordered_map<std::string, TexturePtr>		TextureMap;
	typedef std::unordered_map<std::string, vmml::Vector3f>	Vector3Map;
	typedef std::unordered_map<std::string, GLfloat>		ScalarMap;

	/* Functions */

	/**	@brief Constructor
	*/
	Material() {}

	/**	@brief Virtual destructor
	*/
	virtual ~Material() {}
    
	/**	@brief Initializes the geometry object based on material data and a shader
	*	@param[in] o Object management
	*	@param[in] materialData 
	*	@param[in] shader
	*/
	virtual void initialize(ObjectManager *o, const MaterialData &materialData, ShaderPtr shader);
    
	/**	@brief Bind the shader and pass the attributes of the material
	*/
	virtual void bind();
    
	/**	@brief Returns the textures associated with the material
	*/
	const TextureMap  &getTextures()                      { return _textures; }

	/**	@brief Returns a texture associated with the material
	*	@param[in] name Name of the texture
	*/
    TexturePtr      getTexture(const std::string &name) { return _textures[name];   }
    
	/**	@brief Returns the vectors associated with the material
	*/
	const Vector3Map   &getVectors()                       { return _vectors; }

	/**	@brief Returns a vector associated with the material
	*	@param[in] name Name of the vector
	*/
    vmml::Vector3f     getVector(const std::string &name)  { return _vectors[name];    }
    
	/**	@brief Returns the scalars associated with the material
	*/
	const ScalarMap   &getScalars()                       { return _scalars; }

	/**	@brief Returns a scalar associated with the material
	*	@param[in] name Name of the scalar
	*/
    GLfloat           getScalar(const std::string &name)  { return _scalars[name];    }

	/**	@brief Sets all textures for the material
	*	@param[in] arg Textures for the material
	*/
	void            setTextures(const TextureMap &arg)                            { _textures = arg; }

	/**	@brief Sets a texture for the material
	*	@param[in] name Name of the texture
	*	@param[in] arg Texture for the material
	*/
    void            setTexture(const std::string &name, TexturePtr arg)         { _textures[name] = arg;    }
    
	/**	@brief Sets all vectors for the material
	*	@param[in] arg Vectors for the material
	*/
	void            setVectors(const Vector3Map& arg)                              { _vectors = arg; }

	/**	@brief Sets a vector for the material
	*	@param[in] name Name of the vector
	*	@param[in] arg Vector for the material
	*/
    void            setVector(const std::string &name, const vmml::Vector3f &arg)  { _vectors[name] = arg;     }
    
	/**	@brief Sets all scalars for the material
	*	@param[in] arg Scalar for the material
	*/
	void            setScalars(const ScalarMap &arg)                              { _scalars = arg; }

	/**	@brief Sets a scalar for the material
	*	@param[in] name Name of the scalar
	*	@param[in] arg Scalar for the material
	*/
    void            setScalar(const std::string &name, GLfloat arg)               { _scalars[name] = arg;     }
    
	/**	@brief Returns the shader of the material
	*/
    ShaderPtr   getShader()                 { return _shader;       }

	/**	@brief Sets the shader of the material
	*	@param[in] arg Shader for the material
	*/
    void        setShader(ShaderPtr arg)    { _shader       = arg;  }
    
	/**	@brief Returns the name of the material
	*/
    const std::string &getName()                       { return _name; }

	/**	@brief Sets the name of the material
	*	@param[in] arg Name for the material
	*/
    void        setName(const std::string &arg) { _name = arg;  }
    
private:
	
	/* Variables */

	TextureMap	_textures;
	Vector3Map	_vectors;
	ScalarMap	_scalars;

	ShaderPtr   _shader = nullptr;
    
    std::string _name;
};

typedef std::shared_ptr< Material > MaterialPtr;

#endif /* defined(B_MATERIAL_H) */
