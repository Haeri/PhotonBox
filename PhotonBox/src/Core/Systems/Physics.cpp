#include <algorithm>
#include <iostream>
#include "../../Components/Collider.h"
#include "Physics.h"
#include "../../Components/Rigidbody.h"
#include "../../Components/Transform.h"
#include "../../Core/Entity.h"

std::vector<Collider*> Physics::_colliders;
std::vector<Rigidbody*> Physics::_rigidbodies;
std::map<Transform*, PxRigidDynamic*> Physics::_physXMap;

PxMaterial* Physics::_gMaterial;
PxScene*	Physics::_gScene;
PxPhysics*	Physics::_gPhysics;
PxSceneDesc* Physics::_sceneDesc;

const double Physics::FIXED_TIME_INTERVAL = 1.0f / 60.0f;

void Physics::init()
{
	_gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, _gAllocator, _gErrorCallback);
	if (!_gFoundation)
		std::cerr << "PxCreateFoundation failed!";

	_gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_gFoundation, PxTolerancesScale(), true);

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
	PxRigidStatic* groundPlane = PxCreatePlane(*_gPhysics, PxPlane(0, 1, 0, 0), *_gMaterial);
	_gScene->addActor(*groundPlane);
}

void Physics::update()
{
	_gScene->simulate(FIXED_TIME_INTERVAL);
	_gScene->fetchResults(true);

	PxU32 size = 0;
	const PxActiveTransform* first = _gScene->getActiveTransforms(size);

	for (PxU32  i = 0; i < size; ++i)
	{
		PxTransform pt = first[i].actor2World;
		Transform* t = (static_cast<Entity*>(first[i].actor->userData))->transform;
		t->setPosition(Vector3f(pt.p.x, pt.p.y, pt.p.z));
	}

	/*
	if (_physicsList.size() == 0) return;
	for (int i = 0; i < _physicsList.size() - 1; ++i)
	{
		for (int j = i + 1; j < _physicsList.size(); ++j)
		{
			if (!_physicsList[i]->getEnable() || !_physicsList[j]->getEnable()) return;

			_physicsList[i]->collide(_physicsList[j]);
		}
	}
	*/
}

void Physics::refeed()
{
	for (std::vector<Rigidbody*>::iterator it = _rigidbodies.begin(); it != _rigidbodies.end(); ++it)
	{
		if ((*it)->transform->hasChanged())
		{
			Vector3f p = (*it)->transform->getPositionWorld();
			_physXMap[(*it)->transform]->setGlobalPose(PxTransform(PxVec3(p.getX(), p.getY(), p.getZ())));
		}
	}
}

void Physics::reset()
{
	_gScene->release();

	_gScene = _gPhysics->createScene(*_sceneDesc);
	_gScene->setFlag(PxSceneFlag::eENABLE_ACTIVETRANSFORMS, true);
}

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

void Physics::destroy()
{
	// PHYSX
	_gScene->release();
	_gDispatcher->release();
	_gPhysics->release();
	_gFoundation->release();

	_colliders.clear();
	_rigidbodies.clear();
}

void Physics::addPhysicsObject(Rigidbody* rigidbody) //, Collider* collider)
{
	Matrix4f mat = rigidbody->transform->getTransformationMatrix();
	
	const PxMat44 pmat(mat.getArray());

	PxTransform t(pmat);
	PxRigidDynamic* dynamic = PxCreateDynamic(*_gPhysics, t, PxSphereGeometry(1), *_gMaterial, 10.0f);
	dynamic->userData = rigidbody->entity;
	_gScene->addActor(*dynamic);

	_rigidbodies.push_back(rigidbody);

	_physXMap[rigidbody->entity->transform] = dynamic;
}

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