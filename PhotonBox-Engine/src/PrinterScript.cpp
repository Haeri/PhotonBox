#ifndef PRINTER_SCRIPT_CPP
#define PRINTER_SCRIPT_CPP

#include <iostream>
#include "Components/Behaviour.h"
#include "Components/Transform.h"

class PrinterScript : public Behaviour {
public:
	void Update() {
		std::cout << transform->getPositionWorld() << std::endl;
	}
};

#endif // PRINTER_SCRIPT_CPP
