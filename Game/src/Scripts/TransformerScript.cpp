#ifndef TRANSFORMER_SCRIPT_CPP
#define TRANSFORMER_SCRIPT_CPP

#include <PhotonBox/component/Behaviour.h>
#include <PhotonBox/component/Transform.h>
#include <PhotonBox/core/Time.h>

#ifdef PB_MEM_DEBUG
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