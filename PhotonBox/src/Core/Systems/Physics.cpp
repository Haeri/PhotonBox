#include <algorithm>
#include <iostream>
#include "../../Components/Collider.h"
#include "Physics.h"
#include "../../Components/Rigidbody.h"
#include "../../Components/Transform.h"
#include "../../Core/Entity.h"
#include "../../Components/SphereCollider.h"
#include "../../Math/Math.h"

//std::vector<Collider*> Physics::_colliders;
//std::vector<Rigidbody*> Physics::_rigidbodies;
std::map<Transform*, PxRigidDynamic*> Physics::_physXMap;
std::vector<Rigidbody*> Physics::_initializationList;

PxMaterial*		Physics::_gMaterial;
PxScene*		Physics::_gScene;
PxPhysics*		Physics::_gPhysics;
PxSceneDesc*	Physics::_sceneDesc;

PxPvd*			Physics::_gPvd;
PxPvdTransport*	Physics::_gTransport;

void Physics::init()
{
	_gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, _gAllocator, _gErrorCallback);
	if (!_gFoundation)
		std::cerr << "PxCreateFoundation failed!";

	_gPvd = PxCreatePvd(*_gFoundation);
	_gTransport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
	_gPvd->connect(*_gTransport, PxPvdInstrumentationFlag::eALL);

	_gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_gFoundation, PxTolerancesScale(), true, _gPvd);


	_sceneDesc = new PxSceneDesc(_gPhysics->getTolerancesScale());
	_sceneDesc->gravity = PxVec3(0.0f, -9.81f, 0.0f);
	_gDispatcher = PxDefaultCpuDispatcherCreate(2);
	_sceneDesc->cpuDispatcher = _gDispatcher;
	_sceneDesc->filterShader = PxDefaultSimulationFilterShader;
	
	_gMaterial = _gPhysics->createMaterial(0.5f, 0.5f, 0.6f);	

	_gScene = _gPhysics->createScene(*_sceneDesc);
	_gScene->setFlag(PxSceneFlag::eENABLE_ACTIVETRANSFORMS, true);
}

void Physics::start()
{
	// Base Plane
	PxRigidStatic* groundPlane = PxCreatePlane(*_gPhysics, PxPlane(0, 1, 0, 0), *_gMaterial);
	_gScene->addActor(*groundPlane);

	// Init all objects
	for (std::vector<Rigidbody*>::iterator it = _initializationList.begin(); it != _initializationList.end(); ++it)
	{
		addPhysicsObject((*it));
	}
	_initializationList.clear();
}

void Physics::update(double elapsedTime)
{
	_gScene->simulate(elapsedTime);
	_gScene->fetchResults(true);

	PxU32 size = 0;
	const PxActiveTransform* first = _gScene->getActiveTransforms(size);

	for (PxU32  i = 0; i < size; ++i)
	{
		PxTransform pt = first[i].actor2World;
		Transform* t = (static_cast<Entity*>(first[i].actor->userData))->transform;
		t->setPosition(Vector3f(pt.p.x, pt.p.y, pt.p.z));
		t->setRotation(Math::toEulerAngle(pt.q));
	}
}

void Physics::refeed()
{
	for (std::map<Transform*, PxRigidDynamic*>::iterator it = _physXMap.begin(); it != _physXMap.end(); ++it)
	{
		Transform* t = it->first;
		if (t->hasChanged())
		{
			_physXMap[t]->setGlobalPose(PxTransform(t->getPositionWorld().toPhysX()));
		}
	}
}

void Physics::reset()
{
	_gScene->release();

	_gScene = _gPhysics->createScene(*_sceneDesc);
	_gScene->setFlag(PxSceneFlag::eENABLE_ACTIVETRANSFORMS, true);

	_physXMap.clear();
}

/*
void Physics::addToPhysicsList(Collider *behaviour)
{
	_colliders.push_back(behaviour);
}

void Physics::addToPhysicsList(Rigidbody *rigidbody)
{

	_rigidbodies.push_back(rigidbody);
}

void Physics::removeFromPhysicsList(Collider * collider)
{
	_colliders.erase(std::remove(_colliders.begin(), _colliders.end(), collider), _colliders.end());
}

void Physics::removeFromPhysicsList(Rigidbody * rigidbody)
{
	_rigidbodies.erase(std::remove(_rigidbodies.begin(), _rigidbodies.end(), rigidbody), _rigidbodies.end());
}
*/

void Physics::destroy()
{
	// PHYSX
	_gScene->release();
	_gDispatcher->release();
	_gPhysics->release();
	
	_gPvd->release();
	_gTransport->release();
	_gFoundation->release();

	//_colliders.clear();
	//_rigidbodies.clear();
}

void Physics::addPhysicsObject(Rigidbody* rigidbody) //, Collider* collider)
{
	Matrix4f mat = rigidbody->transform->getTransformationMatrix();
	
	const PxMat44 pmat(mat.getArray());
	Collider* c = rigidbody->entity->getComponent<Collider>();
	if (c == nullptr)
	{
#ifdef _DEBUG
		std::cerr << rigidbody->entity->name << "-Entity with a rigidbody requires a Collider component!\n";
#endif
		return;
	}

	PxGeometry* geo = c->getShape();
	PxTransform t(pmat);
	PxRigidDynamic* dynamic = PxCreateDynamic(*_gPhysics, t, *geo, *_gMaterial, rigidbody->getMass());
	dynamic->userData = rigidbody->entity;
	_gScene->addActor(*dynamic);
	rigidbody->setBody(dynamic);

	//_rigidbodies.push_back(rigidbody);

	_physXMap[rigidbody->entity->transform] = dynamic;
}

void Physics::registerObject(Rigidbody * entity)
{
	_initializationList.push_back(entity);
}

void Physics::removePhysicsObject(Rigidbody * rigidbody)
{
	Transform* t = rigidbody->entity->transform;
	_physXMap[t]->release();
	_physXMap.erase(t);
}

/*
void Physics::printList()
{
	for (std::vector<Collider*>::iterator it = _colliders.begin(); it != _colliders.end(); ++it)
	{
		std::cout << (*it)->getName() << std::endl;
	}
}

std::string Physics::getList()
{
	std::string ret = "";
	for (std::vector<Collider*>::iterator it = _colliders.begin(); it != _colliders.end(); ++it)
	{
		ret += (*it)->getName() + "\n";
	}
	return ret;
}
*/