#ifndef LIGHT_MAP_H
#define LIGHT_MAP_H

#include "PhotonBox/resources/CubeMap.h"

class LightMap
{
public:
	LightMap(const std::vector<std::string>& faces);
	LightMap(CubeMap* cubemap, bool generate = true);
	~LightMap();

	CubeMap* getEnviromentMap();
	CubeMap* getIrradianceMap();
	CubeMap* getSpecularConvolutionMap();

	void generateLightMaps();
private:
	CubeMap* _enviromentMap;
	CubeMap* _irradianceMap;
	CubeMap* _specularMap;
	bool _isManagedEnviromentMap;
};

#endif // LIGHT_MAP_H
