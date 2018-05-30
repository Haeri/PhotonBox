#ifndef PRINTER_SCRIPT_CPP
#define PRINTER_SCRIPT_CPP

#include <Components/Behaviour.h>
#include <Components/Transform.h>
#include <Core/Time.h>

class PrinterScript : public Behaviour
{
public:
	float speed = 1;

	void Update()
	{
		transform->setRotation(Vector3f(0, transform->getRotation().y() + Time::deltaTimef * speed, 0));
	}
};

#endif // PRINTER_SCRIPT_CPP
