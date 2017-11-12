#include <iostream>
#include <algorithm>
#include "Logic.h"
#include "../../Components/Behaviour.h"

std::vector<Behaviour*> Logic::_behaviourList;

void Logic::start() {
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it) {
		(*it)->Start();
	}	
}

void Logic::update() {
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it) {
		if((*it)->getEnable())
			(*it)->Update();
	}
}

void Logic::fixedUpdate() {
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it) {
		if ((*it)->getEnable())
			(*it)->FixedUpdate();
	}
}

void Logic::lateUpdate() {
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it) {
		if ((*it)->getEnable())
			(*it)->LateUpdate();
	}
}

void Logic::destroy() {
	_behaviourList.clear();
}

void Logic::removeFromList(Behaviour* behaviour) {
	_behaviourList.erase(std::remove(_behaviourList.begin(), _behaviourList.end(), behaviour), _behaviourList.end());
}

void Logic::addToUpdateList(Behaviour* behaviour) {
 	_behaviourList.push_back(behaviour);
}

void Logic::printList(){
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}

std::string Logic::getList() {
	std::string ret = "";
	for (std::vector<Behaviour*>::iterator it = _behaviourList.begin(); it != _behaviourList.end(); ++it) {
		ret += " + " + (*it)->getName() + "\n";
	}
	return ret;
}