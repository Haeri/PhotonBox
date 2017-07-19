#include <iostream>
#include <algorithm>
#include "../header/Scene.h"
#include "../header/SceneManager.h"

void Scene::unload() {
	OnUnload();
	for (std::vector<GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end(); ++it) {
		(*it)->destroyComponents();
		delete (*it);
	}
	gameObjectList.clear();
}

void Scene::OnUnload() {}

GameObject* Scene::instanciate(std::string name) {
	GameObject *Go = new GameObject(*this, name);
	addToList(Go);
	Go->parentScene = this;
	return Go;
}

GameObject* Scene::getObjectByName(std::string name) {
    
    std::vector<GameObject*> gameObjects = SceneManager::getCurrentScene()->gameObjectList;
    for (std::vector<GameObject*>::iterator obj = gameObjects.begin() ; obj != gameObjects.end(); ++obj) {
        if ((*obj)->name.find(name) != std::string::npos || (*obj)->name.compare(name) == 0) {
            return *obj;
        }
    }
	return nullptr;
}

void Scene::destroy(GameObject* go) {
	go->destroy();
}

void Scene::printGameObjects() {
	for (std::vector<GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end(); ++it) {
		std::cout << (*it)->name << std::endl;
	}
}

std::string Scene::getGameObjects() {	
	std::string ret = "";
	for (std::vector<GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end(); ++it) {
		ret += (*it)->name + "\n";
	}

	return ret;
}

void Scene::addToList(GameObject* go) {
	gameObjectList.push_back(go);
}

void Scene::removeFromList(GameObject* go) {
	gameObjectList.erase(std::remove(gameObjectList.begin(), gameObjectList.end(), go), gameObjectList.end());
	delete go;
}
