#include "PhotonBox/component/SpotLight.h"

#include "PhotonBox/core/system/Lighting.h"
#include "PhotonBox/resource/shader/ForwardSpotLightShader.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void SpotLight::init()
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

void SpotLight::OnEnable()
{
	Lighting::addLight(this);
}

void SpotLight::OnDisable()
{
	Lighting::removeLight(this);
}