#ifndef B_I_RENDERER_PROJECT_H
#define B_I_RENDERER_PROJECT_H

/* bRenderer includes */
#include "../bRenderer.h"

/** @brief This interface needs to be implemented if the renderer should be used in an object oriented manner
*	@author Benjamin Buergisser
*/
class IRenderProject
{
public:

	/**	@brief Constructor
	*/
	IRenderProject()
	{		
		bRenderer().setRenderProject(this);	// set this instance of RenderProject to be used for function calls
	}

	/**	@brief Virtual destructor
	*/
	virtual ~IRenderProject(){}

	/**	@brief Function that gets called when initializing the renderer
	 */
	virtual void initFunction() = 0;

	/**	@brief Function that gets called repeatedly when looping
	*	@param[in] deltaTime The time past since last frame
	*	@param[in] elapsedTime The time past since initializing the renderer
	*/
	virtual void loopFunction(const double &deltaTime, const double &elapsedTime) = 0;

	/**	@brief Function that gets called when terminating the renderer
	 */
	virtual void terminateFunction() = 0;

protected:

	/**	@brief Returns the project's instance of the renderer
	*/
    Renderer& bRenderer()
    {
		return _renderer;
    }

private:
	Renderer _renderer;
};

#endif /* defined(B_I_RENDERER_PROJECT_H) */
