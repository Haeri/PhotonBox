#include <algorithm>
#include <iostream>
#include "../../Components/Collider.h"
#include "Physics.h"

std::vector<Collider*> Physics::_physicsList;

void Physics::init()
{
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation)
		std::cerr << "PxCreateFoundation failed!";

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);
}

void Physics::update()
{
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);

	if (_physicsList.size() == 0) return;
	for (int i = 0; i < _physicsList.size() - 1; ++i)
	{
		for (int j = i + 1; j < _physicsList.size(); ++j)
		{
			if (!_physicsList[i]->getEnable() || !_physicsList[j]->getEnable()) return;

			_physicsList[i]->collide(_physicsList[j]);
		}
	}
}

void Physics::addToPhysicsList(Collider *behaviour)
{
	_physicsList.push_back(behaviour);
}

void Physics::removeFromPhysicsList(Collider * collider)
{
	_physicsList.erase(std::remove(_physicsList.begin(), _physicsList.end(), collider), _physicsList.end());
}

void Physics::destroy()
{
	// PHYSX
	gScene->release();
	gDispatcher->release();
	gPhysics->release();
	gFoundation->release();

	_physicsList.clear();
}

void Physics::printList()
{
	for (std::vector<Collider*>::iterator it = _physicsList.begin(); it != _physicsList.end(); ++it)
	{
		std::cout << (*it)->getName() << std::endl;
	}
}

std::string Physics::getList()
{
	std::string ret = "";
	for (std::vector<Collider*>::iterator it = _physicsList.begin(); it != _physicsList.end(); ++it)
	{
		ret += (*it)->getName() + "\n";
	}
	return ret;
}
