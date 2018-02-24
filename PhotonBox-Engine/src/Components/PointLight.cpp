#include "../Core/Systems/Lighting.h"
#include "../Resources/ForwardPointLightShader.h"
#include "PointLight.h"

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
