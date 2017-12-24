#include "SceneManager.h"
#include "../../Resources/Scene.h"

bool SceneManager::_inQueue = false;
Scene* SceneManager::_currentScene;
std::string SceneManager::_newScene;
std::map<std::string, Scene*> SceneManager::_sceneMap;
std::string SceneManager::_currentSceneName;

void SceneManager::addScene(const std::string name, Scene* scene){
	_sceneMap[name] = scene;
}

void SceneManager::loadScene(const std::string & name){
	_newScene = name;
	_inQueue = true;
}

void SceneManager::loadSceneImediately(const std::string& name) {
	if (_sceneMap[name] == nullptr) return;
	
	unloadScene(_currentScene);
	_currentScene = _sceneMap[name];
	_currentSceneName = name;
	_currentScene->Load();
	_inQueue = false;
}

bool SceneManager::loadQueuedScene() {
	if (!_inQueue) return false;
	loadSceneImediately(_newScene);
	_newScene = "";
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
	_currentScene->unload();
	for (auto const &scene : _sceneMap) {
		delete scene.second;
	}
	_sceneMap.clear();
}