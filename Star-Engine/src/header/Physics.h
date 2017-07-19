#ifndef PHYSICS_H
#define PHYSICS_H

class Collider;
#include <vector>

class Physics {
public:
	static void update();
	static void addToPhysicsList(Collider *collider);
	static void removeFromPhysicsList(Collider* collider);
	static void destroy();
	static void printList();
	static std::string getList();
private:
	static std::vector<Collider*> physicsList;

};
#endif /* defined(PHYSICS_H) */
