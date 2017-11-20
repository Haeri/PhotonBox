#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

#include "../Core/Component.h"
#include "../Resources/CubeMap.h"
#include "../Core/AABB.h"

class LightProbe : public Component {
public:
	int resolution;
	AABB bounds;

	LightProbe();
	LightProbe(CubeMap* enviromentMap);

	void capture();
	void generateLightMaps();
	CubeMap* getEnviromentCube() { return _envCube; }
	CubeMap* getIrradianceCube() { return _irrCube; }
	CubeMap* getSpecularCube() { return _specCube; }
private:
	CubeMap *_envCube, *_irrCube, *_specCube;
};

#endif // LIGHT_PROBE_H