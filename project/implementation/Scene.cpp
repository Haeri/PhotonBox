#include "Scene.h"

void Scene::unload() {
	onUnload();
	for (std::vector<GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end(); ++it) {
		(*it)->destroy();
		delete *it;
	}
}

void Scene::onUnload() {}

GameObject* Scene::instanciate(std::string name) {
	GameObject *Go = new GameObject(*this, name);
	addToList(Go);
	return Go;
}

void Scene::destroy(GameObject* go) {
	go->destroy();
	removeFromList(go);
	delete go;
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
}