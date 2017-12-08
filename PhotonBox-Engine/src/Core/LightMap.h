#ifndef LIGHT_MAP_H
#define LIGHT_MAP_H

#include "../Resources/CubeMap.h"

class LightMap {
public:
	CubeMap* enviromentMap;
	CubeMap* irradianceMap;
	CubeMap* specularMap;

	LightMap();
	LightMap(CubeMap* enviromentMap): enviromentMap(enviromentMap) { generate(); }
	~LightMap();

	void generate();
};

#endif // LIGHT_MAP_H