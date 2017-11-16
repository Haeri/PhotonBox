#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

#include "../Core/Component.h"
#include "../Resources/CubeMap.h"

class LightProbe : public Component {
public:

	LightProbe(CubeMap* enviromentMap);

	CubeMap* getEnviromentCube() { return _envCube; }
	CubeMap* getIrradianceCube() { return _irrCube; }
	CubeMap* getSpecularCube() { return _specCube; }
private:
	CubeMap *_envCube, *_irrCube, *_specCube;
};

#endif // LIGHT_PROBE_H