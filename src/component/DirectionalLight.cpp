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

void DirectionalLight::init()
{
	Lighting::addLight(this);

	lightProjection = Matrix4f::createOrthographic(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

	_shadowBuffer = new FrameBuffer(4096, 4096);
	_shadowBuffer->addDepthTextureAttachment("depth");
	_shadowBuffer->ready();
}

DirectionalLight::~DirectionalLight()
{
	delete _shadowBuffer;
}

void DirectionalLight::destroy()
{
	Lighting::removeLight(this);
}

Shader* DirectionalLight::getLightShader()
{
	return ForwardDirectionalLightShader::getInstance();
}

FrameBuffer* DirectionalLight::getShadowBuffer()
{
	return _shadowBuffer;
}

void DirectionalLight::OnEnable()
{
	Lighting::addLight(this);
}

void DirectionalLight::OnDisable()
{
	Lighting::removeLight(this);
}