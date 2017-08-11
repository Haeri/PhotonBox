#ifndef PHYSICS_H
#define PHYSICS_H

class Collider;
#include <vector>

class Physics {
public:
	static void addToPhysicsList(Collider *collider);
	static void removeFromPhysicsList(Collider* collider);
	static void printList();
	static std::string getList();

	void update();
	void destroy();
private:
	static std::vector<Collider*> _physicsList;
};

#endif // PHYSICS_H