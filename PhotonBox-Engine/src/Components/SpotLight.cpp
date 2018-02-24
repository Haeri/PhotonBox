#include "../Core/Systems/Lighting.h"
#include "../Resources/ForwardSpotLightShader.h"
#include "SpotLight.h"

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
