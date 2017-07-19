#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class Scene;
#include <map>

class SceneManager {
public:
	static void addScene(const std::string name, Scene* scene);
	static void loadScene(Scene* scene);
	static void loadScene(const std::string &name);
	static void unloadScene(Scene* scene);
	static void unloadScene(const std::string &name);
	static void destroy();
	static std::string getCurrentName();
	static Scene* getCurrentScene();
private:
	static Scene* currentScene;
	static std::map<std::string, Scene*> sceneMap;
	static std::string currentSceneName;
};

#endif /* defined(SCENE_MANAGER_H) */
