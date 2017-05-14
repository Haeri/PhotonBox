#ifndef B_I_DRAWABLE_H
#define B_I_DRAWABLE_H

#include "Renderer_GL.h"

/** @brief Interface for drawable objects
*	@author David Steiner
*/
class IDrawable
{
public:

	/**	@brief Virtual destructor
	*/
	virtual ~IDrawable(){}

	/**	@brief Draws an object to the screen
	*	@param[in] mode
	*/
    virtual void draw(GLenum mode) = 0;

	/**	@brief Draws an instance of the object to the screen
	*	@param[in] instanceName
	*	@param[in] mode
	*/
	virtual void drawInstance(const std::string &instanceName, GLenum mode) = 0;
    
};

typedef std::shared_ptr<IDrawable> DrawablePtr;

#endif /* defined(B_I_DRAWABLE_H) */
