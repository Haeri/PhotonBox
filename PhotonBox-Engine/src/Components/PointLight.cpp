#include "PointLight.h"
#include "../Core/Systems/Lighting.h"
#include "../Resources/ForwardPointLightShader.h"

PointLight::PointLight(){
	Lighting::addLight(this);
	_shader = ForwardPointLightShader::getInstance();
}

void PointLight::destroy(){
	Lighting::removeLight(this);
}
