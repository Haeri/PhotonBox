#include "DirectionalLight.h"
#include "../Core/Systems/Lighting.h"
#include "../Resources/ForwardDirectionalLightShader.h"

DirectionalLight::DirectionalLight(){
	Lighting::addLight(this);
	_shader = ForwardDirectionalLightShader::getInstance();
}

void DirectionalLight::destroy(){
	Lighting::removeLight(this);
}
