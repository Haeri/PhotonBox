#ifndef PRINTER_SCRIPT_CPP
#define PRINTER_SCRIPT_CPP

#include "Components/Behaviour.h"

class PrinterScript : public Behaviour {
public:
	void Update() {
		std::cout << transform->
		//transform->setPosition(Vector3f(transform->getPosition().x() + 0.01f, 0, 0));
		transform->setRotation(Vector3f(0, transform->getRotation().y() + Time::deltaTime * speed, 0));
	}
};

#endif // PRINTER_SCRIPT_CPP
