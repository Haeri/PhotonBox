#include "PhotonBox/component/PointLight.h"

#include "PhotonBox/core/system/Lighting.h"
#include "PhotonBox/resource/shader/ForwardPointLightShader.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

PointLight::PointLight()
{
	Lighting::addLight(this);
}

void PointLight::destroy()
{
	Lighting::removeLight(this);
}

Shader * PointLight::getLightShader()
{
	return ForwardPointLightShader::getInstance();
}

void PointLight::OnEnable()
{
	Lighting::addLight(this);
}

void PointLight::OnDisable()
{
	Lighting::removeLight(this);
}