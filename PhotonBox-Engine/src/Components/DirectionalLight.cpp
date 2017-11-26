#include "DirectionalLight.h"
#include "../Core/Systems/Lighting.h"
#include "../Resources/ForwardDirectionalLightShader.h"
#include "../Resources/DepthShader.h"
#include "../Core/Systems/Renderer.h"
#include "../Resources/DefaultPostShader.h"

DirectionalLight::DirectionalLight(){
	Lighting::addLight(this);
	_shader = ForwardDirectionalLightShader::getInstance();
	_depthShader = DepthShader::getInstance();
	_shadowMapResolution = 2048;
	lightProjection = Matrix4f::createOrthographic(-10, 10, -10, 10, 0.1, 20);

	glGenFramebuffers(1, &_depthMapFBO);

	glGenTextures(1, &_depthMap);
	glBindTexture(GL_TEXTURE_2D, _depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		_shadowMapResolution, _shadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLight::destroy(){
	Lighting::removeLight(this);
}

void DirectionalLight::renderShadowMap(){
	glViewport(0, 0, _shadowMapResolution, _shadowMapResolution);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	Renderer::render(_depthShader);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*
	// Quad Mesh
	static const GLfloat _quadVertices[] = {
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		1.0f,  1.0f,
		1.0f, -1.0f,
	};

	unsigned int _quadVAO;
	unsigned int _quadVBO;
	glGenVertexArrays(1, &_quadVAO);
	glBindVertexArray(_quadVAO);

	glGenBuffers(1, &_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _quadVertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glVertexAttribPointer(Vertex::AttibLocation::POSITION, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(_quadVAO);


	DefaultPostShader* shader = DefaultPostShader::getInstance();
	shader->bind();

	shader->update(nullptr);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _depthMap);

	shader->updateTextures();
	shader->enableAttributes();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	shader->disableAttributes();

	glBindVertexArray(0);
	
	Display::swapBuffer();

	*/
}
