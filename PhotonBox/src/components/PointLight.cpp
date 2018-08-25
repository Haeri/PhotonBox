#include "PhotonBox/components/PointLight.h"

#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/resources/ForwardPointLightShader.h"

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