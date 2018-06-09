#include <limits>
#include <iostream>
#include "PhotonBox/core/FrameBuffer.h"

#include "PhotonBox/resources/DefaultPostShader.h"
#include "PhotonBox/resources/Material.h"
#include "PhotonBox/resources/Vertex.h"
#include "PhotonBox/core/systems/Renderer.h"


GLuint FrameBuffer::_currentFBO;
GLuint FrameBuffer::_quadVAO = -1;
std::vector<FrameBuffer*> FrameBuffer::_bufferList;

std::map<FrameBuffer::InterpolationType, GLint> FrameBuffer::_interpolationTypes = {
	{ NEAREST , GL_NEAREST },
	{ LINEAR ,	GL_LINEAR }
};
std::map<FrameBuffer::InterpolationType, GLint> FrameBuffer::_interpolationMipTypes = {
	{ NEAREST ,			GL_NEAREST_MIPMAP_NEAREST },
	{ LINEAR ,			GL_LINEAR_MIPMAP_LINEAR },
	{ LINEAR_NEAREST ,	GL_LINEAR_MIPMAP_NEAREST},
	{ NEAREST_LINEAR ,	GL_NEAREST_MIPMAP_LINEAR }
};
std::map<FrameBuffer::EdgeType, GLint> FrameBuffer::_edgeTypes = {
	{ CLAMP_TO_EDGE ,			GL_CLAMP_TO_EDGE },
	{ CLAMP_TO_BORDER ,			GL_CLAMP_TO_BORDER },
	{ MIRRORED_REPEAT ,			GL_MIRRORED_REPEAT },
	{ REPEAT ,					GL_REPEAT },
	{ MIRROR_CLAMP_TO_EDGE ,	GL_MIRROR_CLAMP_TO_EDGE }
};

FrameBuffer::FrameBuffer(float screenFactor)
{
	_screenFactor = screenFactor;
	_width = static_cast<int>(Display::getWidth() * screenFactor);
	_height = static_cast<int>(Display::getHeight() * screenFactor);
	initialize();
	_bufferList.push_back(this);
}

FrameBuffer::FrameBuffer(int width, int height)
{
	_screenFactor = -1;
	_width = width;
	_height = height;
	initialize();
	_bufferList.push_back(this);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &_fbo);
	for (auto const &ent1 : _colorAttachments)
	{
		glDeleteRenderbuffers(1, &ent1.second.id);
	}
	glDeleteRenderbuffers(1, &_depthAttachment);
}

void FrameBuffer::addTextureAttachment(std::string name, bool hdr, bool mipmaps, InterpolationType interpolationType, EdgeType edgeType)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	GLint format = hdr ? GL_RGB16F : GL_RGBA;
	BufferAttachment temp(this, name, hdr);

	glGenTextures(1, &temp.id);
	glBindTexture(GL_TEXTURE_2D, temp.id);

	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _edgeTypes[edgeType]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _edgeTypes[edgeType]);

	if (mipmaps)
	{
		temp.mipmaps = static_cast<int>(1 + floor(log2(min(_width, _height))));
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _interpolationMipTypes[interpolationType]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _interpolationTypes[interpolationType]);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		temp.mipmaps = 0;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _interpolationTypes[interpolationType]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _interpolationTypes[interpolationType]);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, _colorAttachmentIndex, GL_TEXTURE_2D, temp.id, 0);
	temp.attachmentIndex = _colorAttachmentIndex;
	_drawBuffers.push_back(_colorAttachmentIndex);

	_colorAttachments[name] = temp;
	++_colorAttachmentIndex;
}

void FrameBuffer::addDepthTextureAttachment(std::string name)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	BufferAttachment temp(this, name, false);

	glGenTextures(1, &temp.id);
	glBindTexture(GL_TEXTURE_2D, temp.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, temp.id, 0);

	temp.mipmaps = 0;
	temp.attachmentIndex = GL_DEPTH_ATTACHMENT;
	_colorAttachments[name] = temp;
}

void FrameBuffer::addDepthBufferAttachment()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glGenRenderbuffers(1, &_depthAttachment);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthAttachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthAttachment);
}


void FrameBuffer::enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glViewport(0, 0, _width, _height);
	_currentFBO = _fbo;
}

void FrameBuffer::bind(GLuint textureUnit, std::string name)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _colorAttachments[name].id);
	if (_colorAttachments[name].mipmaps > 0)
		glGenerateMipmap(GL_TEXTURE_2D);
}

void FrameBuffer::ready()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	if (_drawBuffers.size() > 0)
		glDrawBuffers(static_cast<GLsizei>(_drawBuffers.size()), &_drawBuffers[0]);
	else {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
		
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete: " << fboStatus << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FrameBuffer::render(std::string name)
{
	render(name, nullptr);
}

void FrameBuffer::render(Material* material)
{
	render("", material);
}

void FrameBuffer::resize()
{
	// Only recreate nonstatic buffers
	if (_screenFactor == -1) return;

	glDeleteFramebuffers(1, &_fbo);
	_width = Display::getWidth() * _screenFactor;
	_height = Display::getHeight() * _screenFactor;
	initialize();

	if (_depthAttachment != -1)
	{
		glDeleteRenderbuffers(1, &_depthAttachment);
		addDepthBufferAttachment();
	}

	_drawBuffers.clear();
	for (auto &attachment : _colorAttachments)
	{
		glDeleteRenderbuffers(1, &attachment.second.id);
		if (attachment.second.attachmentIndex == GL_DEPTH_ATTACHMENT)
		{
			addDepthTextureAttachment(attachment.second.name);
		}
		else
		{
			addTextureAttachment(attachment.second.name, attachment.second.hdr, attachment.second.mipmaps);
		}
	}

	ready();
}

FrameBuffer::BufferAttachment* FrameBuffer::getAttachment(std::string name)
{
	return &_colorAttachments[name];
}

void FrameBuffer::render(std::string name, Material* material)
{
	glBindVertexArray(_quadVAO);

	Shader* shader;
	if (material == nullptr)
	{
		shader = DefaultPostShader::getInstance();
	}
	else
	{
		shader = material->shader;
	}

	shader->bind();
	shader->update(nullptr);

	if (material != nullptr)
	{
		material->updateUniforms();
		material->bindTextures();
	}
	else
	{
		bind(GL_TEXTURE0, name);
	}

	shader->updateTextures();
	shader->enableAttributes();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	Renderer::addDrawCall();
	shader->disableAttributes();

	glBindVertexArray(0);
}

void FrameBuffer::initialize()
{
	_colorAttachmentIndex = GL_COLOR_ATTACHMENT0;

	// Create framebuffer
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);


	// Create mesh
	if (_quadVAO == -1)
	{
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

void FrameBuffer::resizeAll()
{
	for (FrameBuffer* fbo : _bufferList)
	{
		fbo->resize();
	}
}

void FrameBuffer::resetDefaultBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Display::getWidth(), Display::getHeight());
	_currentFBO = 0;
}
