#ifndef B_FRAMEBUFFER_H
#define B_FRAMEBUFFER_H

#include <memory>
#include "Renderer_GL.h"
#include "CubeMap.h"
#include "DepthMap.h"

/** @brief A custom framebuffer object
*	@author Benjamin Buergisser
*/
class Framebuffer
{
public:

	/* Functions */

	/**	@brief Constructor that takes care of the resolution automatically
	*/
	Framebuffer();

	/**	@brief Constructor with fixed resolution
	*	@param[in] width
	*	@param[in] height
	*/
	Framebuffer(GLint width, GLint height);

	/**	@brief Virtual destructor
	*/
	virtual ~Framebuffer() { deleteFramebuffer(); }

	/**	@brief Bind the framebuffer object
	*	@param[in] preserveCurrentFramebuffer If true the framebuffer that was active before binding is bound again when unbinding
	*/
	virtual void bind(bool preserveCurrentFramebuffer);

	/**	@brief Bind the framebuffer object and draw to the specified texture
	*	@param[in] texture The texture to draw to
	*	@param[in] preserveCurrentFramebuffer If true the framebuffer that was active before binding is bound again when unbinding
    *
    *   Hint:   Not all textures work, especially when changing resolution between frames. It's best to not use mip mapping.
    *           A suitable texture can be generated using Renderer::createTexture and not passing any data (GLenum format = GL_RGBA, ImageDataPtr imageData = nullptr)
    *			Do NOT use a cube map in this function!
	*/
	virtual void bindTexture(TexturePtr texture, bool preserveCurrentFramebuffer);

	/**	@brief Bind the framebuffer object and draw to the specified cube map 
	*	@param[in] cubeMap The cube map to draw to
	*	@param[in] cubeFace Defines which face to bind: 0 = left, 1 = right, 2 = bottom, 3 = top, 4= front, 5 = back
	*	@param[in] preserveCurrentFramebuffer If true the framebuffer that was active before binding is bound again when unbinding
	*/
	virtual void bindCubeMap(CubeMapPtr cubeMap, GLuint cubeFace, bool preserveCurrentFramebuffer);

	/**	@brief Bind the framebuffer object and draw to the specified depth map
	*	@param[in] depthMap The depth map to draw to
	*	@param[in] preserveCurrentFramebuffer If true the framebuffer that was active before binding is bound again when unbinding
	*/
	virtual void bindDepthMap(DepthMapPtr depthMap, bool preserveCurrentFramebuffer);

	/**	@brief Unbind the framebuffer object
	*/
	virtual void unbind();

	/**	@brief Unbind the framebuffer object and bind the specified framebuffer object
	*	@param[in] fbo The framebuffer object that should be bound after unbinding
	*/
	virtual void unbind(GLint fbo);

	/**	@brief Updates the size of the framebuffer
	*	@param[in] width
	*	@param[in] height
     *	@param[in] autoResize Set true if the size should be reset to the viewport size automatically the next time the fbo is bound
	*/
	virtual void resize(GLint width, GLint height, bool autoResize);
    
    /**	@brief Get the framebuffer id
     */
    GLuint getFramebufferID()       { return _fbo; }
    
    /**	@brief Get the depthbuffer id
     */
    GLuint getDebthbufferID()       { return _rbo_depth; }
    
    /**	@brief Get the width of the framebuffer
     */
    GLint getWidth()                { return _width; }
    
    /**	@brief Get the height of the framebuffer
     */
    GLint getHeight()               { return _height; }

	/**	@brief Get the framebuffer currently active
	*/
	static GLint getCurrentFramebuffer();

	/**	@brief Delete the framebuffer
	*/
	virtual void deleteFramebuffer();

private:

	/* Functions */

	virtual void create();
	virtual void bindBuffer(bool preserveCurrentFramebuffer);

	/* Variables */
	GLint _width, _height, _oldFbo = 0;
	GLuint _fbo = 0, _rbo_depth = 0;
	bool _preserveCurrentFramebuffer, _autoResize;

};

typedef std::shared_ptr<Framebuffer> FramebufferPtr;

#endif /* defined(B_FRAMEBUFFER_H) */
