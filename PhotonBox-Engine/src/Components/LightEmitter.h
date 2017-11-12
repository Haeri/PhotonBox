#ifndef LIGHT_EMITTER_H
#define LIGHT_EMITTER_H

#include "../Core/Component.h"
#include "../Math/Vector3f.h"

class LightEmitter : public Component {
public:
	Vector3f color;
	float intensity;

	//LightEmitter();

	//void destroy() override;
};

#endif // LIGHT_EMITTER_H
