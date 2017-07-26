#include <vector>
#include <algorithm>
#include <iostream>
#include "../header/Physics.h"
#include "../header/Collider.h"

void Physics::update() {
	if (physicsList.size() == 0) return;
	for (int i = 0; i < physicsList.size() - 1; ++i) {
		for (int j = i+1; j < physicsList.size(); ++j) {
			if (!physicsList[i]->getEnable() || !physicsList[j]->getEnable()) return;

			physicsList[i]->collide(physicsList[j]);
		}
	}
}

void Physics::addToPhysicsList(Collider *behaviour) {
	physicsList.push_back(behaviour);
}

void Physics::removeFromPhysicsList(Collider * collider){
	physicsList.erase(std::remove(physicsList.begin(), physicsList.end(), collider), physicsList.end());
}

void Physics::destroy(){
	physicsList.clear();
}

void Physics::printList() {
	for (std::vector<Collider*>::iterator it = physicsList.begin(); it != physicsList.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}

std::string Physics::getList() {
	std::string ret = "";
	for (std::vector<Collider*>::iterator it = physicsList.begin(); it != physicsList.end(); ++it) {
		ret += (*it)->getName() + "\n";
	}
	return ret;
}