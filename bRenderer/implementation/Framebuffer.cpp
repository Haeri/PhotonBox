#include "headers/Framebuffer.h"
#include "headers/Logger.h"
#include "headers/OSdetect.h"

/* Constructors */

Framebuffer::Framebuffer()
	: _preserveCurrentFramebuffer(false), _autoResize(true)
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	_width = vp[2];
	_height = vp[3];

	create();
}

Framebuffer::Framebuffer(GLint width, GLint height)
	: _width(width), _height(height), _preserveCurrentFramebuffer(false), _autoResize(false)
{	
	create();
}

/* Public Functions */

void Framebuffer::bind(bool preserveCurrentFramebuffer)
{
	bindBuffer(preserveCurrentFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		bRenderer::log("Framebuffer status: " + std::to_string(status), bRenderer::LM_ERROR);
	}
}

void Framebuffer::bindTexture(TexturePtr texture, bool preserveCurrentFramebuffer)
{
	bindBuffer(preserveCurrentFramebuffer);
	// Resize and reset texture
	texture->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getTextureID(), 0);
	// Important: glClear has to be called after binding the texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		bRenderer::log("Framebuffer status: " + std::to_string(status), bRenderer::LM_ERROR);
	}
}

void Framebuffer::bindCubeMap(CubeMapPtr cubeMap, GLuint cubeFace, bool preserveCurrentFramebuffer)
{
	bindBuffer(preserveCurrentFramebuffer);
	// Resize and reset texture 
	cubeMap->bind();
	GLsizei size = (_width > _height) ? _width : _height;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeFace, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeFace, cubeMap->getTextureID(), 0);
	// Important: glClear has to be called after binding the texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		bRenderer::log("Framebuffer status: " + std::to_string(status), bRenderer::LM_ERROR);
	}
}

void Framebuffer::bindDepthMap(DepthMapPtr depthMap, bool preserveCurrentFramebuffer)
{
	bindBuffer(preserveCurrentFramebuffer);
	// Resize and reset texture
	depthMap->bind();
#ifdef B_OS_DESKTOP
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _width, _height, 0, GL_DEPTH_COMPONENT16, GL_FLOAT, 0);
#endif
#ifdef B_OS_IOS
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _width, _height, 0, GL_DEPTH_COMPONENT16, GL_UNSIGNED_SHORT, 0);
#endif	
#ifdef B_OS_DESKTOP
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap->getTextureID(), 0);
	// No color output is needed
	glDrawBuffer(GL_NONE);
#endif
#ifdef B_OS_IOS
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap->getTextureID(), 0);
#endif
	// Important: glClear has to be called after binding the texture
	glClear(GL_DEPTH_BUFFER_BIT);

	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		bRenderer::log("Framebuffer status: " + std::to_string(status), bRenderer::LM_ERROR);
	}
}

void Framebuffer::unbind()
{
#ifdef B_OS_DESKTOP
	glDrawBuffer(GL_BACK);
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (_preserveCurrentFramebuffer)
		glBindFramebuffer(GL_FRAMEBUFFER, _oldFbo);
}

void Framebuffer::unbind(GLint fbo)
{
	unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

GLint Framebuffer::getCurrentFramebuffer()
{
	GLint currentFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo);
	return currentFbo;
}

void Framebuffer::resize(GLint width, GLint height, bool autoResize)
{
    _autoResize = autoResize;
	if (_width != width || _height != height){
		_width = width;
		_height = height;

		// depthbuffer
		glBindRenderbuffer(GL_RENDERBUFFER, _rbo_depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

void Framebuffer::deleteFramebuffer()
{
	if (_rbo_depth)
		glDeleteRenderbuffers(1, &_rbo_depth);
	if (_fbo)
		glDeleteFramebuffers(1, &_fbo);

}

/* Private Functions */

void Framebuffer::create()
{
	// depthbuffer
	glGenRenderbuffers(1, &_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// framebuffer
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rbo_depth);
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		bRenderer::log("Framebuffer status: " + std::to_string(status), bRenderer::LM_ERROR);
	}

	// clear
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// unbind
	unbind();
}
void Framebuffer::bindBuffer(bool preserveCurrentFramebuffer)
{
	_preserveCurrentFramebuffer = preserveCurrentFramebuffer;
	if (_preserveCurrentFramebuffer)
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_oldFbo);

	if (_autoResize){
		GLint vp[4];
		glGetIntegerv(GL_VIEWPORT, vp);
		resize(vp[2], vp[3], _autoResize);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}