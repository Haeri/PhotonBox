#include "PhotonBox/core/LazyLoadable.h"

#include <thread>
#include <iostream>

#include "PhotonBox/core/system/ResourceManager.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void LazyLoadable::loadAsync()
{
	blankInitialize();

	ResourceManager::addToInitializationList(this);
	std::thread{ &LazyLoadable::load, this }.detach();
}

void LazyLoadable::forceLoad()
{
	blankInitialize();
	load();
	initialize();
}

void LazyLoadable::setRogue()
{
	_rogue = true;
}

void LazyLoadable::initialize()
{
	if(!_failedToLoad)
	{
		submitBuffer();
		_isInitialized = true;
	}
}

bool LazyLoadable:: isLoaded()
{
	return _isLoaded && !_failedToLoad; 
}

bool LazyLoadable::isInitialized()
{
	return _isInitialized; 
}

std::string LazyLoadable::getFilePath()
{
	return _filePath.getAbsolutePath();
}

void LazyLoadable::load()
{
	_failedToLoad = !loadFromFile();
	_isLoaded = true;

	if (_rogue) {
		delete this;
	}
}
