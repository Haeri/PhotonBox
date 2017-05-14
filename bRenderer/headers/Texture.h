#ifndef B_TEXTURE_H
#define B_TEXTURE_H

#include <memory>
#include <string>
#include "Renderer_GL.h"

class TextureData;

/** @brief Texture
*	@author Rahul Mukhi, David Steiner
*/
class Texture
{
public:

	/* Functions */

	/**	@brief Constructor
	*
	*	This constructor generates a texture yet doesn't apply any settings or data
	*/
	Texture() { glGenTextures(1, &_textureID); }

	/**	@brief Constructor
	*	@param[in] data The texture data to be used
	*/
    Texture(const TextureData &data);

	/**	@brief Constructor
	*
	*	This constructor should only be used if the texture is already created (e.g. by an external library)
	*	and should be integrated in the framework
	*
	*	@param[in] textureID 
	*/
	Texture(GLuint textureID):_textureID(textureID){}

	/**	@brief Virtual destructor
	*/
	virtual ~Texture() { deleteTexture(); }
    
	/**	@brief Returns texture id
	*/
	GLuint getTextureID()	{ return _textureID; }

	/**	@brief Binds the texture
	*	@param[in] texUnit Specifies which texture unit to bind it to
	*/
    virtual void bind(GLint texUnit = GL_TEXTURE0);

	/**	@brief Delete the OpenGL texture
	*/
	virtual void deleteTexture()
	{
		if (_textureID)
			glDeleteTextures(1, &_textureID);
	}
    
private:

	/* Variables */

    GLuint _textureID = 0;
};

typedef std::shared_ptr< Texture >  TexturePtr;

#endif /* defined(B_TEXTURE_H) */
