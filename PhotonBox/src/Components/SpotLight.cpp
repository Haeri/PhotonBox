#include "PhotonBox/components/SpotLight.h"

#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/resources/ForwardSpotLightShader.h"

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
