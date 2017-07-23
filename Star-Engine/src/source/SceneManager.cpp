#include "../header/SceneManager.h"
#include "../header/Scene.h"
#include "../header/Renderer.h"
#include "../header/Core.h"
#include "../header/Logic.h"

void SceneManager::addScene(const std::string name, Scene* scene){
	sceneMap[name] = scene;
}

void SceneManager::loadScene(Scene * scene){
	unloadScene(currentScene);
    scene->Load();
	currentScene = scene;
}


void SceneManager::loadScene(const std::string & name){
	loadScene(sceneMap[name]);
	currentSceneName = name;
}

void SceneManager::unloadScene(Scene * scene){
	if (currentScene == nullptr) return;

	scene->unload();
	currentScene = nullptr;
}

void SceneManager::unloadScene(const std::string & name){
	unloadScene(sceneMap[name]);
}

std::string SceneManager::getCurrentName() {
	return currentSceneName;
}

Scene* SceneManager::getCurrentScene() {
	return currentScene;
}

void SceneManager::destroy(){

	for (auto const &scene : sceneMap) {
		scene.second->unload();
		delete scene.second;
	}
}
