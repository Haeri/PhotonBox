#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class Scene;
#include <map>

class SceneManager {
public:
	void addScene(const std::string name, Scene* scene);
	void loadScene(Scene* scene);
	void loadScene(const std::string &name);
	void unloadScene(Scene* scene);
	void unloadScene(const std::string &name);
	void destroy();
	std::string getCurrentName();
	Scene* getCurrentScene();
private:
	Scene* currentScene;
	std::map<std::string, Scene*> sceneMap;
	std::string currentSceneName;
};

#endif /* defined(SCENE_MANAGER_H) */
