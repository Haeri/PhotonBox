#ifndef B_CUBE_MAP_H
#define B_CUBE_MAP_H

#include <vector>
#include "Texture.h"

/** @brief Cube map 
*	@author Benjamin Buergisser
*/
class CubeMap : public Texture
{
public:

	/* Functions */

	/**	@brief Constructor
	*	@param[in] data The texture data to be used (size needs to be 6) in the following order: 
	*	left, right, bottom, top, front,  back
	*/
	CubeMap(const std::vector<TextureData> &data);

	/**	@brief Virtual destructor
	*/
	virtual ~CubeMap() { deleteTexture(); }

	/**	@brief Binds the cube map
	*	@param[in] texUnit Specifies which texture unit to bind it to
	*/
	virtual void bind(GLint texUnit = GL_TEXTURE0) override;

};

typedef std::shared_ptr< CubeMap >  CubeMapPtr;

#endif /* defined(B_CUBE_MAP_H) */
