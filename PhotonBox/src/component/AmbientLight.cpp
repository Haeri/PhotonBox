#include "PhotonBox/component/AmbientLight.h"

#include "PhotonBox/core/system/Lighting.h"
#include "PhotonBox/resource/shader/ForwardAmbientLightShader.h"

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
