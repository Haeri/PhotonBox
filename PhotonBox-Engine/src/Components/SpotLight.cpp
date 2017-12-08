#include "SpotLight.h"
#include "../Core/Systems/Lighting.h"
#include "../Resources/ForwardSpotLightShader.h"

SpotLight::SpotLight(){
	Lighting::addLight(this);
}

void SpotLight::destroy(){
	Lighting::removeLight(this);
}

Shader * SpotLight::getLightShader(){
	return ForwardSpotLightShader::getInstance();
}
