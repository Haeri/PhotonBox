#ifndef PRINTER_SCRIPT_CPP
#define PRINTER_SCRIPT_CPP

#include <PhotonBox/component/Behaviour.h>
#include <PhotonBox/component/Transform.h>
#include <PhotonBox/core/Time.h>

#ifdef PB_MEM_DEBUG
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
