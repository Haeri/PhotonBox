#include <vector>
#include <iostream>
#include "Physics.h"
#include "Collider.h"

std::vector<Collider*> Physics::physicsList;

void Physics::update() {
	if (physicsList.size() == 0) return;
	for (int i = 0; i < physicsList.size() - 1; ++i) {
		for (int j = i+1; j < physicsList.size(); ++j) {
			physicsList[i]->collide(physicsList[j]);
		}
	}
}

void Physics::addToPhysicsList(Collider *behaviour) {
	physicsList.push_back(behaviour);
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