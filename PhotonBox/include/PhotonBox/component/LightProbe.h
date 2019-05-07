#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

class CubeMap;
class LightMap;

#include "PhotonBox/core/Component.h"
#include "PhotonBox/data-type/AABB.h"

class LightProbe : public Component
{
public:
	int resolution;
	AABB bounds;

	void init() override;
	void capture();
	CubeMap* getEnviromentCube();
	CubeMap* getIrradianceCube();
	CubeMap* getSpecularCube();

	void destroy() override;
private:
	int _steps = 4;
	LightMap* _lightMap;

	LightMap* captureRecursive(int step);
};

#endif // LIGHT_PROBE_H
