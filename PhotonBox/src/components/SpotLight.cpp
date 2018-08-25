#include "PhotonBox/components/SpotLight.h"

#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/resources/ForwardSpotLightShader.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

SpotLight::SpotLight()
{
	Lighting::addLight(this);
}

void SpotLight::destroy()
{
	Lighting::removeLight(this);
}

Shader * SpotLight::getLightShader()
{
	return ForwardSpotLightShader::getInstance();
}

void SpotLight::OnEnable()
{
	Lighting::addLight(this);
}

void SpotLight::OnDisable()
{
	Lighting::removeLight(this);
}