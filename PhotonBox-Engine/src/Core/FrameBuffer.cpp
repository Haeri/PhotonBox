#include "FrameBuffer.h"

#include <limits>
#include <iostream>
#include "../Resources/Material.h"
#include "../Resources/Vertex.h"
#include "../Resources/DefaultPostShader.h"

GLuint FrameBuffer::_currentFBO;
GLuint FrameBuffer::_quadVAO = -1;

//#define DEBUG

FrameBuffer::FrameBuffer(int width, int height) : FrameBuffer(width, height, false) {}
FrameBuffer::FrameBuffer(int width, int height, bool mipmaps) : FrameBuffer(width, height, false, true) {}
FrameBuffer::FrameBuffer(int width, int height, bool mipmaps, bool hdr) : _width(width), _height(height), _isHDR(hdr) {
	GLint format = hdr ? GL_RGB16F : GL_RGBA;

	// Create framebuffer
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	

	// Create texture to hold color buffer
	glGenTextures(1, &_texColor);
	glBindTexture(GL_TEXTURE_2D, _texColor);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (mipmaps) {
		_maxMipMaps = 1 + floor(log2(min(width, height)));
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		_maxMipMaps = 0;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texColor, 0);


	// Create Renderbuffer Object to hold depth and stencil buffers
	glGenRenderbuffers(1, &_rbDS);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbDS);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbDS);



	// Error check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error while createing framebuffer" << std::endl;

	

	if (_quadVAO == -1){
		// Quad Mesh
		static const GLfloat _quadVertices[] = {
			-1.0f,  1.0f,
			-1.0f, -1.0f,
			1.0f,  1.0f,
			1.0f, -1.0f,
		};

		glGenVertexArrays(1, &_quadVAO);
		glBindVertexArray(_quadVAO);

		glGenBuffers(1, &_quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _quadVertices, GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
		glVertexAttribPointer(Vertex::AttibLocation::POSITION, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
}

void FrameBuffer::addTextureAttachment(std::string name) 
{
	addTextureAttachment(name, false, false);
}

void FrameBuffer::addTextureAttachment(std::string name, bool hdr, bool drawBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	GLint format = hdr ? GL_RGB16F : GL_RGBA;
	_colorAttachments[name] = 0;
	GLuint* texture = &(_colorAttachments[name]);

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, _colorAttachmentIndex, GL_TEXTURE_2D, *texture, 0);

	if (drawBuffer)
		_drawBuffers.push_back(_colorAttachmentIndex);

	++_colorAttachmentIndex;
}

void FrameBuffer::addDepthTextureAttachment(std::string name) 
{
	addDepthTextureAttachment(name, false);
}

void FrameBuffer::addDepthTextureAttachment(std::string name, bool drawBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	_colorAttachments[name] = 0;
	GLuint* texture = &(_colorAttachments[name]);

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *texture, 0);

	if (drawBuffer)
		_drawBuffers.push_back(GL_DEPTH_ATTACHMENT);
}

void FrameBuffer::addDepthBufferAttachment()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glGenRenderbuffers(1, &_depthAttachment);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthAttachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthAttachment);
}


void FrameBuffer::enable() {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glViewport(0, 0, _width, _height);
	_currentFBO = _fbo;
}

void FrameBuffer::bind() {
	bind(GL_TEXTURE0);
}

void FrameBuffer::bind(GLuint textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _texColor);
	if (_maxMipMaps > 0)
		glGenerateMipmap(GL_TEXTURE_2D);
}

void FrameBuffer::bind(GLuint textureUnit, std::string name) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _colorAttachments[name]);
	if (_maxMipMaps > 0)
		glGenerateMipmap(GL_TEXTURE_2D);
}

void FrameBuffer::finish()
{
	if (_drawBuffers.size() > 0)
		glDrawBuffers(_drawBuffers.size(), &_drawBuffers[0]);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Error while createing framebuffer!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// TODO: clear only necessary attachments
void FrameBuffer::clear(){
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FrameBuffer::render() {
	render(nullptr);
}

void FrameBuffer::render(Material* material) {
	glBindVertexArray(_quadVAO);

	Shader* shader;
	if (material == nullptr) {
		shader = DefaultPostShader::getInstance();
	}else {
		shader = material->shader;
	}

	shader->bind();
	shader->update(nullptr);

	if (material != nullptr) {
		material->updateUniforms();
		material->bindTextures();
	}else {
		bind();
	}

	shader->updateTextures();
	shader->enableAttributes();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	shader->disableAttributes();

	glBindVertexArray(0);


#ifdef DEBUG
	if (_currentFBO == 0) return;

		FrameBuffer::resetDefaultBuffer();
		this->render();
		Display::swapBuffer();
		system("PAUSE");
#endif
}

void FrameBuffer::render(GLuint texId) {
	glBindVertexArray(_quadVAO);

	Shader* shader;
	shader = DefaultPostShader::getInstance();
	

	shader->bind();
	shader->update(nullptr);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);
	
	shader->updateTextures();
	shader->enableAttributes();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	shader->disableAttributes();

	glBindVertexArray(0);
}

void FrameBuffer::destroy() {
	glDeleteFramebuffers(1, &_fbo);
	glDeleteRenderbuffers(1, &_rbDS);
}

void FrameBuffer::resetDefaultBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Display::getWidth(), Display::getHeight());
	_currentFBO = 0;
}