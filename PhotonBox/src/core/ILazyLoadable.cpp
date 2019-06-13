#include "PhotonBox/core/ILazyLoadable.h"

#include <thread>
#include <iostream>

#include "PhotonBox/core/system/ResourceManager.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void ILazyLoadable::loadAsync()
{
	_isLoaded = false;
	_isInitialized = false;
	
	blankInitialize();

	ResourceManager::addToInitializationList(this);
	std::thread{ &ILazyLoadable::load, this }.detach();
}

void ILazyLoadable::forceLoad()
{
	blankInitialize();
	load();
	initialize();
}

void ILazyLoadable::setRogue()
{
	_rougue = true;
}

void ILazyLoadable::initialize()
{
	submitBuffer();
	_isInitialized = true;

}

void ILazyLoadable::load()
{
	loadFromFile();
	_isLoaded = true;

	if (_rougue) {
		delete this;
	}
}
