#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "PhotonBox/core/Entity.h"

class Scene
{
public:
	std::vector<Entity*> entityList;

	virtual void Load() = 0;
	virtual void OnUnload();
	void unload();

	Entity* instanciate(std::string name);
	void destroy(Entity* go);
	void printEntitys();
	std::string getEntitys();
	void removeFromList(Entity* go);
	Entity* getObjectByName(std::string name);
private:
	void addToList(Entity* go);
};

#endif /* defined(SCENE_H) */
