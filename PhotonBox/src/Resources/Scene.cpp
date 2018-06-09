#include <iostream>
#include <algorithm>
#include "PhotonBox/resources/Scene.h"

#include "PhotonBox/core/systems/Core.h"
#include "PhotonBox/core/systems/SceneManager.h"

void Scene::unload()
{
	OnUnload();
	for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		(*it)->destroyComponents();
		delete (*it);
	}
	entityList.clear();
}

void Scene::OnUnload() {}

Entity* Scene::instanciate(std::string name)
{
	Entity *Go = new Entity(*this, name);
	addToList(Go);
	Go->parentScene = this;
	return Go;
}

Entity* Scene::getObjectByName(std::string name)
{

	std::vector<Entity*> entitys = SceneManager::getCurrentScene()->entityList;
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

void Scene::printEntitys()
{
	for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		std::cout << (*it)->name << std::endl;
	}
}

std::string Scene::getEntitys()
{
	std::string ret = "";
	for (std::vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		ret += " + " + (*it)->name + "\n";
	}

	return ret;
}

void Scene::addToList(Entity* go)
{
	entityList.push_back(go);
}

void Scene::removeFromList(Entity* go)
{
	entityList.erase(std::remove(entityList.begin(), entityList.end(), go), entityList.end());
	delete go;
}
