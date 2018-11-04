#ifndef TRANSFORMER_SCRIPT_CPP
#define TRANSFORMER_SCRIPT_CPP

#include <Components/Behaviour.h>
#include <Components/Transform.h>
#include <Core/Time.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class TransformerScript : public Behaviour
{
public:
	float speed = 0.3f;

	void Update()
	{
		//transform->setRotation(Vector3f(0, transform->getRotation().y() + Time::deltaTime * speed, 0));
	}
};

#endif // TRANSFORMER_SCRIPT_CPP