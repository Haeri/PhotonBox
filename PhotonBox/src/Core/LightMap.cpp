#include "PhotonBox/core/LightMap.h"

LightMap::LightMap() {}

LightMap::~LightMap()
{
	delete enviromentMap;
	delete irradianceMap;
	delete specularMap;
}

void LightMap::generate()
{
	irradianceMap = new CubeMap(32);
	irradianceMap->generateIrradiance(enviromentMap->getLocation());

	specularMap = new CubeMap(128, true);
	specularMap->generateSpecularConvolution(enviromentMap->getLocation());
}
