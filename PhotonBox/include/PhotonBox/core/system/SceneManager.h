#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class Scene;

#include <map>

#include "PhotonBox/resource/Scene.h"
#include "PhotonBox/core/ISystem.h"

class SceneManager : public ISystem
{
public:
	static void addScene(const std::string name, Scene* scene);
	static void loadScene(const std::string &name);
	static void unloadScene(Scene* scene);
	static void unloadScene(const std::string &name);
	static std::string getCurrentName();
	static Scene* getCurrentScene();
	static void setSceneMap(std::map<std::string, Scene*>& sceneMap);

	void start() override;
	void destroy() override;

	void loadSceneImmediately(const std::string &name);
	void loadQueuedScene();
	bool sceneQueued() { return _inQueue; }
	void drawSceneList();
private:
	static std::string _newScene;
	static std::string _currentSceneName;
	static bool _inQueue;
	static Scene* _currentScene;
	static std::map<std::string, Scene*> _sceneMap;
};

#endif // SCENE_MANAGER_H
