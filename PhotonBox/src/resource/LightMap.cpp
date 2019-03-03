#include "PhotonBox/resource/LightMap.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

LightMap::LightMap(const std::vector<std::string>& allFaces)
{
	_enviromentMap = new CubeMap(allFaces);
	_isManagedEnviromentMap = false;
	generateLightMaps();
}

LightMap::LightMap(CubeMap* cubemap, bool generate)
{
	_enviromentMap = cubemap;
	_isManagedEnviromentMap = true;
	if(generate)
		generateLightMaps();
}

LightMap::~LightMap()
{
	if(!_isManagedEnviromentMap)
		delete _enviromentMap;
	delete _irradianceMap;
	delete _specularMap;
}

CubeMap* LightMap::getEnviromentMap() {
	return _enviromentMap;
}

CubeMap* LightMap::getIrradianceMap() {
	return _irradianceMap;
}

CubeMap* LightMap::getSpecularConvolutionMap() {
	return _specularMap;
}

void LightMap::generateLightMaps()
{
	_irradianceMap = new CubeMap(32);
	_irradianceMap->generateIrradiance(_enviromentMap->getLocation());

	_specularMap = new CubeMap(128, true);
	_specularMap->generateSpecularConvolution(_enviromentMap->getLocation());
}
