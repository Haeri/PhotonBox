#ifndef PHYSICS_H
#define PHYSICS_H

class Collider;
#include <vector>

class Physics {
public:
	void update();
	void addToPhysicsList(Collider *collider);
	void removeFromPhysicsList(Collider* collider);
	void destroy();
	void printList();
	std::string getList();
private:
	std::vector<Collider*> physicsList;

};
#endif /* defined(PHYSICS_H) */
