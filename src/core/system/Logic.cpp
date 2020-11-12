#include "PhotonBox/core/system/Logic.h"

#include <iostream>
#include <algorithm>

#include "PhotonBox/component/Behaviour.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

std::vector<Behaviour*> Logic::_behaviourList;
unsigned long int ::Logic::_tickIndex;

void Logic::start()
{
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it)
	{
		(*it)->Start();
	}
}

void Logic::reset()
{
	_tickIndex = 0;
}

void Logic::update()
{
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it)
	{
		if ((*it)->getEnable())
			(*it)->Update();
	}
}

void Logic::tick()
{
	++_tickIndex;
}

void Logic::fixedUpdate()
{
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it)
	{
		if ((*it)->getEnable())
			(*it)->FixedUpdate();
	}
}

void Logic::lateUpdate()
{
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it)
	{
		if ((*it)->getEnable())
			(*it)->LateUpdate();
	}
}

void Logic::destroy()
{
	_behaviourList.clear();
}

void Logic::removeFromList(Behaviour* behaviour)
{
	_behaviourList.erase(std::remove(_behaviourList.begin(), _behaviourList.end(), behaviour), _behaviourList.end());
}

void Logic::addToUpdateList(Behaviour* behaviour)
{
	_behaviourList.push_back(behaviour);
}

unsigned long int Logic::getTickIndex()
{
	return _tickIndex;
}