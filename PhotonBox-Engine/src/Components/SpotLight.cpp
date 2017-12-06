#include "SpotLight.h"
#include "../Core/Systems/Lighting.h"
#include "../Resources/ForwardSpotLightShader.h"

SpotLight::SpotLight(){
	Lighting::addLight(this);
	_shader = ForwardSpotLightShader::getInstance();
}

void SpotLight::destroy(){
	Lighting::removeLight(this);
}