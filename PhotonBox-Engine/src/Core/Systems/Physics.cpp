#include <algorithm>
#include <iostream>
#include "Physics.h"
#include "../../Components/Collider.h"

std::vector<Collider*> Physics::_physicsList;

void Physics::update() {
	if (_physicsList.size() == 0) return;
	for (int i = 0; i < _physicsList.size() - 1; ++i) {
		for (int j = i+1; j < _physicsList.size(); ++j) {
			if (!_physicsList[i]->getEnable() || !_physicsList[j]->getEnable()) return;

			_physicsList[i]->collide(_physicsList[j]);
		}
	}
}

void Physics::addToPhysicsList(Collider *behaviour) {
	_physicsList.push_back(behaviour);
}

void Physics::removeFromPhysicsList(Collider * collider){
	_physicsList.erase(std::remove(_physicsList.begin(), _physicsList.end(), collider), _physicsList.end());
}

void Physics::destroy(){
	_physicsList.clear();
}

void Physics::printList() {
	for (std::vector<Collider*>::iterator it = _physicsList.begin(); it != _physicsList.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}

std::string Physics::getList() {
	std::string ret = "";
	for (std::vector<Collider*>::iterator it = _physicsList.begin(); it != _physicsList.end(); ++it) {
		ret += (*it)->getName() + "\n";
	}
	return ret;
}
