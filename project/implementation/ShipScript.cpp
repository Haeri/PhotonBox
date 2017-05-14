#ifndef SHIP_SCRIPT_H
#define SHIP_SCRIPT_H

#include "Core.h"
#include "Behaviour.h"
#include "TheTime.h"
#include "TheRenderer.h"

class ShipScript : public Behaviour {
public:
	void Update() {
		transform->position = TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->getPosition() * -1;
	}
};

#endif /* defined(SHIP_SCRIPT_H) */