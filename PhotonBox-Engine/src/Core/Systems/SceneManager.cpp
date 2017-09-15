#include "SceneManager.h"
#include "../../Resources/Scene.h"

Scene* SceneManager::_currentScene;
Scene* SceneManager::_toBeLoadedScene;
std::map<std::string, Scene*> SceneManager::_sceneMap;
std::string SceneManager::_currentSceneName;

void SceneManager::addScene(const std::string name, Scene* scene){
	_sceneMap[name] = scene;
}

void SceneManager::loadScene(Scene * scene){
	_toBeLoadedScene = scene;
}

void SceneManager::loadScene(const std::string & name){
	loadScene(_sceneMap[name]);
}

void SceneManager::loadSceneImediately(Scene* scene) {
	unloadScene(_currentScene);
	scene->Load();
	_currentScene = scene;
	// TODO: save scene names correctly
	//_currentSceneName = _sceneMap.->name;
}

void SceneManager::loadSceneImediately(const std::string& name) {
	loadSceneImediately(_sceneMap[name]);
}

bool SceneManager::loadQueuedScene() {
	if (_toBeLoadedScene == nullptr) return false;
	loadSceneImediately(_toBeLoadedScene);
	_toBeLoadedScene = nullptr;
	return true;
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
