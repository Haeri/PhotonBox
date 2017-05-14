#ifndef PHYSICS_H
#define PHYSICS_H

class Collider;
#include <vector>

class Physics {
public:
	void update();
	static void addToPhysicsList(Collider *collider);
	static void removeFromPhysicsList(Collider* collider);
	static void printList();
	static std::string getList();
private:
	static std::vector<Collider*> physicsList;

};
#endif /* defined(PHYSICS_H) */
