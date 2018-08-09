#ifndef LIGHT_MAP_H
#define LIGHT_MAP_H

#include "PhotonBox/resources/CubeMap.h"

class LightMap
{
public:
	CubeMap* enviromentMap;
	CubeMap* irradianceMap;
	CubeMap* specularMap;

	LightMap() {}
	LightMap(const std::vector<std::string>& faces);
	LightMap(CubeMap* cubemap);
	~LightMap();

	void generateLightMaps();
};

#endif // LIGHT_MAP_H
