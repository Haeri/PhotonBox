#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class Scene;
#include <map>

class SceneManager
{
public:
	static void addScenes(std::map<std::string, Scene*>& sceneMap);
	static void addScene(const std::string name, Scene* scene);
	static void loadScene(const std::string &name);
	static void unloadScene(Scene* scene);
	static void unloadScene(const std::string &name);
	static std::string getCurrentName();
	static Scene* getCurrentScene();

	void loadSceneImediately(const std::string &name);
	void loadQueuedScene();
	bool sceneQueued() { return _inQueue; }
	void destroy();
	void drawSceneList();
private:
	static std::string _newScene;
	static std::string _currentSceneName;
	static bool _inQueue;
	static Scene* _currentScene;
	static std::map<std::string, Scene*> _sceneMap;
};

#endif // SCENE_MANAGER_H
