#include "PhotonBox/core/system/SceneManager.h"

#include "PhotonBox/resource/Scene.h"
#include "PhotonBox/core/Display.h"
#include "PhotonBox/core/system/DebugGUI.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

bool SceneManager::_inQueue = false;
Scene* SceneManager::_currentScene;
std::string SceneManager::_newScene;
std::map<std::string, Scene*> SceneManager::_sceneMap;
std::string SceneManager::_currentSceneName;

void SceneManager::setSceneMap(std::map<std::string, Scene*>& sceneMap)
{
	_sceneMap = sceneMap;
	loadScene(_sceneMap.begin()->first);
}

void SceneManager::start()
{
	loadQueuedScene();
}

void SceneManager::addScene(const std::string name, Scene* scene)
{
	_sceneMap[name] = scene;
}

void SceneManager::loadScene(const std::string & name)
{
	_newScene = name;
	_inQueue = true;
}

void SceneManager::loadSceneImediately(const std::string& name)
{
	if (_sceneMap[name] == nullptr) return;

	unloadScene(_currentScene);
	_currentScene = _sceneMap[name];
	_currentSceneName = name;
	_currentScene->Load();
	Display::setTitle(Config::profile.appName + " - " + name);
	_inQueue = false;
}

void SceneManager::loadQueuedScene()
{
	loadSceneImediately(_newScene);
	_newScene = "";
}

void SceneManager::unloadScene(Scene * scene)
{
	if (_currentScene == nullptr) return;

	scene->unload();
	_currentScene = nullptr;
}

void SceneManager::unloadScene(const std::string & name)
{
	unloadScene(_sceneMap[name]);
}

std::string SceneManager::getCurrentName()
{
	return _currentSceneName;
}

Scene* SceneManager::getCurrentScene()
{
	return _currentScene;
}

void SceneManager::destroy()
{
	_currentScene->unload();
	for (auto const &scene : _sceneMap)
	{
		delete scene.second;
	}
	_sceneMap.clear();
}

void SceneManager::drawSceneList()
{
	ImGui::Begin("Scenes");
	for (auto const &scene : _sceneMap)
	{
		if (ImGui::Button(scene.first.c_str()))
		{
			loadScene(scene.first);
		}
	}
	ImGui::End();
}
