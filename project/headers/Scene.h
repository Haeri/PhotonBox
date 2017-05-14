#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "GameObject.h"

class Scene {
public:
	std::vector<GameObject*> gameObjectList;

	virtual void load() =0;
	virtual void onUnload();
	void unload();

	GameObject* instanciate(std::string name);
	void destroy(GameObject* go);
	void printGameObjects();
	std::string getGameObjects();
private:
	void addToList(GameObject* go);
	void removeFromList(GameObject* go);
};

#endif /* defined(SCENE_H) */