#include "PhotonBox/components/AmbientLight.h"

#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/resources/ForwardAmbientLightShader.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

AmbientLight::AmbientLight()
{
	Lighting::addLight(this);
}

void AmbientLight::destroy()
{
	Lighting::removeLight(this);
}

Shader* AmbientLight::getLightShader()
{
	return ForwardAmbientLightShader::getInstance();
}
