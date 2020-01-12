#include "PhotonBox/core/system/Physics.h"

#include <algorithm>
#include <iostream>

#include "PhotonBox/core/Entity.h"
#include "PhotonBox/math/Math.h"
#include "PhotonBox/component/Collider.h"
#include "PhotonBox/component/Rigidbody.h"
#include "PhotonBox/component/Transform.h"
#include "PhotonBox/component/SphereCollider.h"
#include "PhotonBox/util/Logger.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

//std::vector<Collider*> Physics::_colliders;
//std::vector<Rigidbody*> Physics::_rigidbodies;
std::map<Transform*, PxRigidDynamic*> Physics::_physXMap;
std::vector<Rigidbody*> Physics::_initializationList;

PxMaterial*		Physics::_gMaterial;
PxScene*		Physics::_gScene;
PxPhysics*		Physics::_gPhysics;
PxSceneDesc*	Physics::_sceneDesc;

#ifdef PDV_DEBUG
PxPvd*			Physics::_gPvd;
PxPvdTransport*	Physics::_gTransport;
#endif // PDV_DEBUG

void Physics::init(Config::Profile profile)
{
	Logger::infoln("Initializing Physics");

	_gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, _gAllocator, _gErrorCallback);
	if (!_gFoundation)
		Logger::errln("PxCreateFoundation failed!");

#ifdef PDV_DEBUG
	_gPvd = PxCreatePvd(*_gFoundation);
	_gTransport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
	_gPvd->connect(*_gTransport, PxPvdInstrumentationFlag::eALL);

	_gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_gFoundation, PxTolerancesScale(), true, _gPvd);
#else
	_gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_gFoundation, PxTolerancesScale(), true);
#endif // PDV_DEBUG



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

	/*
	for (std::vector<Rigidbody*>::iterator it = _initializationList.begin(); it != _initializationList.end(); ++it)
	{
		addPhysicsObject((*it));
	}
	_initializationList.clear();
	*/
}

void Physics::update(double elapsedTime)
{
	_gScene->simulate(static_cast<physx::PxReal>(elapsedTime));
	_gScene->fetchResults(true);

	PxU32 size = 0;
	const PxActiveTransform* first = _gScene->getActiveTransforms(size);

	for (PxU32  i = 0; i < size; ++i)
	{
		PxTransform pt = first[i].actor2World;
		Transform* t = (static_cast<Entity*>(first[i].actor->userData))->transform;
		t->setPosition(Vector3f(pt.p.x, pt.p.y, pt.p.z));
		t->setRotation(Quaternion(pt.q.x, pt.q.y, pt.q.z, pt.q.w));
	}
}

void Physics::refeed()
{
	
	for (std::map<Transform*, PxRigidDynamic*>::iterator it = _physXMap.begin(); it != _physXMap.end(); ++it)
	{
		Transform* t = it->first;
		//if (t->hasChangedLastTick())
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
	delete _sceneDesc;

#ifdef PDV_DEBUG
	_gPvd->release();
	_gTransport->release();
#endif // PDV_DEBUG

	_gFoundation->release();

	//_colliders.clear();
	//_rigidbodies.clear();
}

void Physics::addPhysicsObject(Rigidbody* rigidbody) //, Collider* collider)
{
	/*
	Matrix4f mat = rigidbody->transform->getTransformationMatrix();
	
	const PxMat44 pmat(mat.getArray());
	Collider* c = rigidbody->entity->getComponent<Collider>();
	if (c == nullptr)
	{
#ifdef _DEBUG
		Logger::errln(rigidbody->entity->name ,"-Entity with a rigidbody requires a Collider component!");
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
	*/
}

void Physics::registerObject(Rigidbody * entity)
{
	_gScene->addActor(*entity->getBody());
	_physXMap[entity->getTransform()] = (PxRigidDynamic*)entity->getBody();


//	_initializationList.push_back(entity);
}

void Physics::removePhysicsObject(Rigidbody * rigidbody)
{
	Transform* t = rigidbody->getEntity()->transform;
	_physXMap[t]->release();
	_physXMap.erase(t);
}

PxPhysics * Physics::getPhysX()
{
	return _gPhysics;
}

/*
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
