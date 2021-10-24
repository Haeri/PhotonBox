#ifndef PHYSICS_H
#define PHYSICS_H

// "Exactly one of NDEBUG and _DEBUG needs to be defined!" for non Windows platforms
#ifndef _WINDOWS
	#ifdef DEBUG
		#define _DEBUG
	#else
		#define NDEBUG
	#endif
#endif

class Collider;
class Rigidbody;
class Entity;
class Transform;

#include <vector>
#include <map>

#include "PhotonBox/core/ISystem.h"

#define PX_PHYSX_STATIC_LIB
#include "PxPhysicsAPI.h"

using namespace physx;

class Physics : public ISystem
{
public:
	//static void addToPhysicsList(Collider *collider);
	//static void addToPhysicsList(Rigidbody *rigidbody);
	//static void removeFromPhysicsList(Collider* collider);
	//static void removeFromPhysicsList(Rigidbody *rigidbody);

	static void registerObject(Rigidbody* entity);
	static void removePhysicsObject(Rigidbody* rigidbody);
	static PxPhysics* getPhysX();
	//static void printList();
	//static std::string getList();

	void init(Config::Profile profile) override;
	void start() override;
	void reset() override;
	void destroy() override;

	void update(double elapsedTime);
	void refeed();
private:
	//static std::vector<Collider*> _colliders;
	//static std::vector<Rigidbody*> _rigidbodies;
	static std::map<Transform*, PxRigidDynamic*> _physXMap;
	static std::vector<Rigidbody*> _initializationList;

	PxDefaultAllocator		_gAllocator;
	PxDefaultErrorCallback	_gErrorCallback;
	PxFoundation*			_gFoundation;
	PxDefaultCpuDispatcher*	_gDispatcher;
	static PxPvd*			_gPvd;
	static PxPvdTransport*	_gTransport;

	static PxPhysics*		_gPhysics;
	static PxScene*			_gScene;
	static PxMaterial*		_gMaterial;
	static PxSceneDesc*		_sceneDesc;
	
	void addPhysicsObject(Rigidbody* rigidbody); // , Collider* collider);
};

#endif // PHYSICS_H
