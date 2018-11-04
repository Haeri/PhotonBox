#ifndef PRINTER_SCRIPT_CPP
#define PRINTER_SCRIPT_CPP

#include <Components/Behaviour.h>
#include <Components/Transform.h>
#include <Core/Time.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class PrinterScript : public Behaviour
{
public:
	float speed = 1;

	void Update()
	{
		//
	}
};

#endif // PRINTER_SCRIPT_CPP
