#ifndef PHYSICS_H
#define PHYSICS_H

class Collider;
#include <vector>


#include "PxPhysicsAPI.h"

using namespace physx;

class Physics
{
public:
	static void addToPhysicsList(Collider *collider);
	static void removeFromPhysicsList(Collider* collider);
	static void printList();
	static std::string getList();

	void init();
	void update();
	void destroy();
private:
	static std::vector<Collider*> _physicsList;



	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;

	PxFoundation*			gFoundation = NULL;
	PxPhysics*				gPhysics = NULL;

	PxDefaultCpuDispatcher*	gDispatcher = NULL;
	PxScene*				gScene = NULL;

	PxMaterial*				gMaterial = NULL;
};

#endif // PHYSICS_H
