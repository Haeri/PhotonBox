#include "PhotonBox/component/Collider.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Collider::Collider()
{
	//Physics::addToPhysicsList(this);
}

void Collider::destroy()
{
	//Physics::removeFromPhysicsList(this);
}
