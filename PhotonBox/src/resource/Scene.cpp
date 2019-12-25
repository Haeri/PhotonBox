#include "PhotonBox/resource/Scene.h"

#include <iostream>
#include <algorithm>

#include "PhotonBox/core/Core.h"
#include "PhotonBox/core/Entity.h"
#include "PhotonBox/core/ManagedResource.h"
#include "PhotonBox/core/system/SceneManager.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void Scene::unload()
{
	OnUnload();
	for (std::vector<Entity*>::iterator it = _entityList.begin(); it != _entityList.end(); ++it)
	{
		(*it)->destroyComponents();
		delete (*it);
	}
	_entityList.clear();

	for (std::map<std::string, ManagedResource*>::iterator it = _resourceMap.begin(); it != _resourceMap.end(); ++it)
	{
		LazyLoadable* f = dynamic_cast<LazyLoadable*>(it->second);
		if (f != nullptr) {
			if (!f->isLoaded()) 
			{
				f->setRogue(); 
			}
			else
			{
				delete (it->second);
			}
		}
	}
	_resourceMap.clear();
}

void Scene::OnUnload() {}

Entity* Scene::instantiate(std::string name)
{
	Entity *entity = new Entity(*this, name);
	addToList(entity);
	entity->parentScene = this;
	return entity;
}

Entity* Scene::getObjectByName(std::string name)
{
	std::vector<Entity*> entitys = SceneManager::getCurrentScene()->_entityList;
	for (std::vector<Entity*>::iterator obj = entitys.begin(); obj != entitys.end(); ++obj)
	{
		if ((*obj)->name.find(name) != std::string::npos || (*obj)->name.compare(name) == 0)
		{
			return *obj;
		}
	}
	return nullptr;
}

void Scene::destroy(Entity* go)
{
	go->destroy();
}

void Scene::addToList(Entity* go)
{
	_entityList.push_back(go);
}

void Scene::removeFromList(Entity* go)
{
	_entityList.erase(std::remove(_entityList.begin(), _entityList.end(), go), _entityList.end());
	delete go;
}

std::vector<Entity*>& Scene::getEntities()
{
	return _entityList;
}