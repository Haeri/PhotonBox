#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "LightEmitter.h"

class PointLight : public LightEmitter {
public:
	float attenuation;
};
#endif /* defined(POINT_LIGHT_H) */