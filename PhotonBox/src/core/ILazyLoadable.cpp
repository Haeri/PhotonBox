#include "PhotonBox/core/ILazyLoadable.h"

#include <thread>

#include "PhotonBox/core/system/ResourceManager.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void ILazyLoadable::loadAsync()
{
	_isLoaded = false;
	_isInitialized = false;
	
	blankInitialize();

	ResourceManager::addToInitializationList(this);
	std::thread{ &ILazyLoadable::loadFromFile, this }.detach();
}

void ILazyLoadable::forceLoad()
{
	blankInitialize();
	loadFromFile();
	sendToGPU();
}