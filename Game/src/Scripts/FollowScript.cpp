#ifndef FOLLOW_SCRIPT_CPP
#define FOLLOW_SCRIPT_CPP

#include <component/Behaviour.h>
#include <component/Transform.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class FollowScript : public Behaviour
{
public:
	Transform * target;

	void Update()
	{
		target->setPosition(transform->getPosition());
	}
};

#endif // FOLLOW_SCRIPT_CPP