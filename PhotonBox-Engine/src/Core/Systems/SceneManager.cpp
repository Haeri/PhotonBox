#include "SceneManager.h"
#include "../../Resources/Scene.h"

Scene* SceneManager::_currentScene;
std::map<std::string, Scene*> SceneManager::_sceneMap;
std::string SceneManager::_currentSceneName;

void SceneManager::addScene(const std::string name, Scene* scene){
	_sceneMap[name] = scene;
}

void SceneManager::loadScene(Scene * scene){
	unloadScene(_currentScene);
    scene->Load();
	_currentScene = scene;
}

void SceneManager::loadScene(const std::string & name){
	loadScene(_sceneMap[name]);
	_currentSceneName = name;
}

void SceneManager::unloadScene(Scene * scene){
	if (_currentScene == nullptr) return;

	scene->unload();
	_currentScene = nullptr;
}

void SceneManager::unloadScene(const std::string & name){
	unloadScene(_sceneMap[name]);
}

std::string SceneManager::getCurrentName() {
	return _currentSceneName;
}

Scene* SceneManager::getCurrentScene() {
	return _currentScene;
}

void SceneManager::destroy(){

	for (auto const &scene : _sceneMap) {
		scene.second->unload();
		delete scene.second;
	}
}
