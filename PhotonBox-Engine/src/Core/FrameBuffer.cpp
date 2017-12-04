#include "FrameBuffer.h"

#include <iostream>
#include "../Resources/Material.h"
#include "../Resources/Vertex.h"
#include "../Resources/DefaultPostShader.h"

GLuint FrameBuffer::_currentFBO;

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

	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texColor);
	*/

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

void FrameBuffer::destroy() {
	glDeleteFramebuffers(1, &_fbo);
	glDeleteRenderbuffers(1, &_rbDS);
}

void FrameBuffer::resetDefaultBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Display::getWidth(), Display::getHeight());
	_currentFBO = 0;
}