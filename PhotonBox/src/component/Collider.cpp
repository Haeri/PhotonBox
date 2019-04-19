#include "PhotonBox/component/Collider.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

std::type_index Collider::getBaseType()
{
	return typeid(Collider); 
}