#include "PhotonBox/component/Behaviour.h"

#include "PhotonBox/core/system/Logic.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void Behaviour::init()
{
	Logic::addToUpdateList(this);
}

void Behaviour::destroy()
{
	OnDestroy();
	Logic::removeFromList(this);
}

void Behaviour::Start() {}
void Behaviour::Update() {}
void Behaviour::FixedUpdate() {}
void Behaviour::LateUpdate() {}
void Behaviour::OnDestroy() {}
