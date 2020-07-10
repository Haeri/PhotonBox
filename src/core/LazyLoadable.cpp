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
	_state = State::LOADING;
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
	if(_state != State::FAILED)
	{
		submitBuffer();
		_state = State::INITIALIZED;
	}
}

LazyLoadable::State LazyLoadable::getState()
{
	return _state;
}

bool LazyLoadable::isLoaded()
{
	return _state == State::LOADED;
}

bool LazyLoadable::isInitialized()
{
	return _state == State::INITIALIZED; 
}

std::string LazyLoadable::getFilePath()
{
	return _filePath.getAbsolutePath();
}

void LazyLoadable::load()
{
	_state = loadFromFile() ? State::LOADED : State::FAILED;

	if (_rogue) {
		delete this;
	}
}
