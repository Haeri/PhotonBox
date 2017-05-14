#ifndef B_RENDERQUEUE_H
#define B_RENDERQUEUE_H

#include <memory>
#include <map>

#include "Renderer_GL.h"
#include "Shader.h"
#include "Material.h"
#include "Properties.h"
#include "IDrawable.h"

struct RenderCall
{
	DrawablePtr drawable;
	std::string instanceName;
	GLenum blendSfactor, blendDfactor;
};

/** @brief A queue that stores render calls and performs drawing in sorted order
*	@author Benjamin Buergisser
*/
class RenderQueue : public IDrawable
{
public:
	/* Typedefs */
	typedef std::multimap< std::string, RenderCall> RenderCallsOpaque;
	typedef std::multimap< GLfloat, RenderCall> RenderCallsTransparent;

	/* Functions */

	/**	@brief Constructor
	*/
	RenderQueue(){}

	/**	@brief Virtual destructor
	*/
	virtual ~RenderQueue(){}

	/**	@brief Add a render call to the queue
	*	@param[in] programID
	*	@param[in] materialName
	*	@param[in] drawableName
	*	@param[in] instanceName
	*	@param[in] drawable
	*	@param[in] distanceToCamera The distance of the transparent drawable to the camera (optional)
	*	@param[in] isTransparent Set true if the object is transparent (optional)
	*	@param[in] blendSfactor Specifies how the red, green, blue, and alpha source blending factors are computed (optional)
	*	@param[in] blendDfactor Specifies how the red, green, blue, and alpha destination blending factors are computed (optional)
	*/
	virtual void submitToRenderQueue(GLuint programID, const std::string &materialName, const std::string &drawableName, const std::string &instanceName, DrawablePtr drawable, GLfloat distanceToCamera = 0.0, bool isTransparent = false, GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA);

	/**	@brief Draw all drawable in the queue in a sorted manner
	*/
	virtual void draw(GLenum mode = GL_TRIANGLES) override;

	/**	@brief Draws an instance of the queue to the screen
	*	@param[in] instanceName
	*	@param[in] mode
	*/
	virtual void drawInstance(const std::string &instanceName, GLenum mode) override { draw(mode); }

	/**	@brief Remove all render calls
	*/
	virtual void clear();


private:

	/* Variables */
	
	RenderCallsOpaque			_renderCallsOpaque;
	RenderCallsTransparent		_renderCallsTransparent;

};

typedef std::shared_ptr<RenderQueue> RenderQueuePtr;

#endif /* defined(B_RENDERQUEUE_H) */
