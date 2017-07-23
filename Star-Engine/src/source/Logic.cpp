#include <iostream>
#include <algorithm>
#include "../header/Logic.h"
#include "../header/Behaviour.h"

void Logic::start() {
	for (std::vector<Behaviour*>::iterator it = behaviourList.begin(); it != behaviourList.end(); ++it) {
		(*it)->Start();
	}
}

void Logic::update() {
	for (std::vector<Behaviour*>::iterator it = behaviourList.begin(); it != behaviourList.end(); ++it) {
		if((*it)->getEnable())
			(*it)->Update();
	}
}

void Logic::fixedUpdate() {
	for (std::vector<Behaviour*>::iterator it = behaviourList.begin(); it != behaviourList.end(); ++it) {
		if ((*it)->getEnable())
			(*it)->FixedUpdate();
	}
}

void Logic::lateUpdate() {
	for (std::vector<Behaviour*>::iterator it = behaviourList.begin(); it != behaviourList.end(); ++it) {
		if ((*it)->getEnable())
			(*it)->LateUpdate();
	}
}

void Logic::destroy() {
	behaviourList.clear();
}

void Logic::removeFromList(Behaviour* behaviour) {
	behaviourList.erase(std::remove(behaviourList.begin(), behaviourList.end(), behaviour), behaviourList.end());
}

void Logic::addToUpdateList(Behaviour* behaviour) {
 	behaviourList.push_back(behaviour);
}

void Logic::printList(){
	for (std::vector<Behaviour*>::iterator it = behaviourList.begin(); it != behaviourList.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}

std::string Logic::getList() {
	std::string ret = "";
	for (std::vector<Behaviour*>::iterator it = behaviourList.begin(); it != behaviourList.end(); ++it) {
		ret += (*it)->getName() + "\n";
	}
	return ret;
}