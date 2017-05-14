#ifndef B_SPRITE_H
#define B_SPRITE_H

#include "Model.h"

/** @brief A 2d plane that can be rendered to the screen
*	@author Benjamin Buergisser
*/
class Sprite : public Model
{
public:
	/* Functions */

	/**	@brief Constructor
	*/
	Sprite(){}

	/**	@brief Constructor
	*	@param[in] material
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	Sprite(MaterialPtr material, bool flipT, PropertiesPtr	properties = nullptr);

	/**	@brief Constructor
	*	@param[in] o Object management
	*	@param[in] textureFileName The filename of the texture that should be loaded and displayed
	*	@param[in] materialName The name of the material that should be created
	*	@param[in] shader
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	Sprite(ObjectManager *o, const std::string &textureFileName, const std::string &materialName, ShaderPtr shader, bool flipT, PropertiesPtr properties = nullptr);

	/**	@brief Constructor
	*	@param[in] o Object management
	*	@param[in] name The name for the shader and material that will be generated
	*	@param[in] textureFileName The filename of the texture that should be loaded and displayed
	*	@param[in] shaderMaxLights The maximum number of light sources to be used 
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	Sprite(ObjectManager *o, const std::string &name, const std::string &textureFileName, GLuint shaderMaxLights, bool variableNumberOfLights, bool flipT, PropertiesPtr properties = nullptr);

	/**	@brief Virtual destructor
	*/
	virtual ~Sprite() { deleteModelGeometry(); }

private:

	/* Functions */
	
	void createGeometry(bool flipT);

};

typedef std::shared_ptr< Sprite >  SpritePtr;

#endif /* defined(B_SPRITE_H) */
