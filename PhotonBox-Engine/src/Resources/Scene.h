#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "../Core/GameObject.h"

class Scene
{
public:
	std::vector<GameObject*> gameObjectList;

	virtual void Load() = 0;
	virtual void OnUnload();
	void unload();

	GameObject* instanciate(std::string name);
	void destroy(GameObject* go);
	void printGameObjects();
	std::string getGameObjects();
	void removeFromList(GameObject* go);
	GameObject* getObjectByName(std::string name);
private:
	void addToList(GameObject* go);
};

#endif /* defined(SCENE_H) */
