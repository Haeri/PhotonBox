#include "PhotonBox/component/DirectionalLight.h"

#include "PhotonBox/resource/FrameBuffer.h"
#include "PhotonBox/core/system/Lighting.h"
#include "PhotonBox/core/system/Renderer.h"
#include "PhotonBox/resource/shader/DefaultPostShader.h"
#include "PhotonBox/resource/shader/DirectionalShadowShader.h"
#include "PhotonBox/resource/shader/ForwardDirectionalLightShader.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

DirectionalLight::DirectionalLight()
{
	Lighting::addLight(this);

	lightProjection = Matrix4f::createOrthographic(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
}

DirectionalLight::~DirectionalLight()
{
	//shadowBuffer->clear();
}

void DirectionalLight::destroy()
{
	Lighting::removeLight(this);
}

Shader* DirectionalLight::getLightShader()
{
	return ForwardDirectionalLightShader::getInstance();
}

/*
void DirectionalLight::renderShadowMap(bool captureMode)
{
	shadowBuffer->enable();
	shadowBuffer->clear();

	//Renderer::render(_depthShader, captureMode);

	
	if(!captureMode){
		ImGui::Begin("Depth");
		ImGui::Image((ImTextureID)shadowBuffer->getAttachment("depth")->id, ImVec2(_shadowMapResolution/15.0f, _shadowMapResolution/15.0f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
	
	

	//FrameBuffer::resetDefaultBuffer();
}
*/


void DirectionalLight::OnEnable()
{
	Lighting::addLight(this);
}

void DirectionalLight::OnDisable()
{
	Lighting::removeLight(this);
}