#include "PhotonBox/components/DirectionalLight.h"

#include "PhotonBox/core/FrameBuffer.h"
#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/core/systems/Renderer.h"
#include "PhotonBox/resources/DefaultPostShader.h"
#include "PhotonBox/resources/DepthShader.h"
#include "PhotonBox/resources/ForwardDirectionalLightShader.h"

#include "imgui\imgui.h"

DirectionalLight::DirectionalLight()
{
	Lighting::addLight(this);
	_depthShader = DepthShader::getInstance();
	_shadowMapResolution = 4096;
	lightProjection = Matrix4f::createOrthographic(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

	shadowBuffer = new FrameBuffer(_shadowMapResolution, _shadowMapResolution);
	shadowBuffer->addDepthTextureAttachment("depth");
	shadowBuffer->ready();
}

DirectionalLight::~DirectionalLight()
{
	shadowBuffer->clear();
}

void DirectionalLight::destroy()
{
	Lighting::removeLight(this);
}

Shader* DirectionalLight::getLightShader()
{
	return ForwardDirectionalLightShader::getInstance();
}

void DirectionalLight::renderShadowMap(bool captureMode)
{
	shadowBuffer->enable();
	shadowBuffer->clear();

	Renderer::render(_depthShader, captureMode);

	
	if(!captureMode){
		ImGui::Begin("Depth");
		ImGui::Image((void *)shadowBuffer->getAttachment("depth")->id, ImVec2(_shadowMapResolution/15.0f, _shadowMapResolution/15.0f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
	

	FrameBuffer::resetDefaultBuffer();
}
