#include "PhotonBox/components/PointLight.h"

#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/resources/ForwardPointLightShader.h"

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
