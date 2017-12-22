#include "DirectionalLight.h"
#include "../Core/Systems/Lighting.h"
#include "../Resources/DepthShader.h"
#include "../Core/Systems/Renderer.h"
#include "../Resources/DefaultPostShader.h"
#include "../Resources/ForwardDirectionalLightShader.h"
#include "../Core/FrameBuffer.h"

DirectionalLight::DirectionalLight(){
	Lighting::addLight(this);
	_depthShader = DepthShader::getInstance();
	_shadowMapResolution = 2048;
	lightProjection = Matrix4f::createOrthographic(-5, 5, -5, 5, 0.1, 100);

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

Shader * DirectionalLight::getLightShader(){
	return ForwardDirectionalLightShader::getInstance();
}

void DirectionalLight::renderShadowMap(bool captureMode){
	glViewport(0, 0, _shadowMapResolution, _shadowMapResolution);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	Renderer::render(_depthShader, captureMode);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
