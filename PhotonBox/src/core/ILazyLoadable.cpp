#include "PhotonBox/core/ILazyLoadable.h"

#include <thread>

#include "PhotonBox/core/systems/ResourceManager.h"

void ILazyLoadable::loadAsync()
{
	_isLoaded = false;
	_isInitialized = false;
	
	blankInitialize();

	ResourceManager::addToInitializationList(this);
	std::thread{ &ILazyLoadable::loadFromFile, this }.detach();
}
