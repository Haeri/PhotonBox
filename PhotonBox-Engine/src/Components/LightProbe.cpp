#include "LightProbe.h"

LightProbe::LightProbe(CubeMap* enviromentMap){
	_envCube = enviromentMap;
	
	_irrCube = new CubeMap(32);
	_irrCube->generateIrradiance(_envCube->getLocation());

	_specCube = new CubeMap(128, true);
	_specCube->generateSpecularConvolution(_envCube->getLocation());
}
