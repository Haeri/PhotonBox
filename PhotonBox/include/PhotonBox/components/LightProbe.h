#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

class CubeMap;
class LightMap;

#include "PhotonBox/core/AABB.h"
#include "PhotonBox/core/Component.h"

class LightProbe : public Component
{
public:
	int resolution;
	AABB bounds;

	LightProbe(int steps = 3);

	void capture();
	CubeMap* getEnviromentCube();
	CubeMap* getIrradianceCube();
	CubeMap* getSpecularCube();

	void destroy() override;
private:
	int _steps;
	LightMap* _lightMap;

	LightMap* captureRecursive(int step);
};

#endif // LIGHT_PROBE_H
