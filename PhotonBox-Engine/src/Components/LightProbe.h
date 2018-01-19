#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

#include "../Core/Component.h"
#include "../Resources/CubeMap.h"
#include "../Core/AABB.h"
#include "../Core/LightMap.h"

class LightProbe : public Component
{
public:
	int resolution;
	AABB bounds;

	LightProbe();
	LightProbe(CubeMap* enviromentMap);

	void generateLightMap();
	LightMap* captureAmbient(int pass, LightMap* lastLightMap);
	void capture();
	LightMap* captureRecursive(int step);
	CubeMap* getEnviromentCube() { return _lightMap.enviromentMap; }
	CubeMap* getIrradianceCube() { return _lightMap.irradianceMap; }
	CubeMap* getSpecularCube() { return _lightMap.specularMap; }

	void destroy() override;
private:
	LightMap _lightMap;
};

#endif // LIGHT_PROBE_H