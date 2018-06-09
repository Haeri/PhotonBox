#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

#include "PhotonBox/core/AABB.h"
#include "PhotonBox/core/Component.h"
#include "PhotonBox/core/LightMap.h"
#include "PhotonBox/resources/CubeMap.h"

class LightProbe : public Component
{
public:
	int resolution;
	AABB bounds;

	LightProbe();
	LightProbe(CubeMap* enviromentMap);

	void generateLightMap();
	void capture();
	LightMap* captureAmbient(int pass, LightMap* lastLightMap);
	LightMap* captureRecursive(int step);
	CubeMap* getEnviromentCube() { return _lightMap.enviromentMap; }
	CubeMap* getIrradianceCube() { return _lightMap.irradianceMap; }
	CubeMap* getSpecularCube() { return _lightMap.specularMap; }

	void destroy() override;
private:
	LightMap _lightMap;
};

#endif // LIGHT_PROBE_H
