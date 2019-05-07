#ifndef SCENE_H
#define SCENE_H

class ManagedResource;

#include <vector>

#include "PhotonBox/core/Entity.h"
#include "PhotonBox/resource/CubeMap.h"
#include "PhotonBox/resource/Mesh.h"
#include "PhotonBox/resource/Texture.h"

class Scene
{
public:
	virtual void Load() = 0;
	virtual void OnUnload();
	void unload();

	Entity* instanciate(std::string name);
	template <class T, typename ...Args>
	T* createResource(Args ...args) 
	{
		T *res = new T(args...);
		_resourceList.push_back(res);
		return res;
	}
	void destroy(Entity* go);
	void printEntitys();
	std::string getEntitys();
	void removeFromList(Entity* go);
	Entity* getObjectByName(std::string name);
	std::vector<Entity*>& getEntities() { return _entityList; }
private:
	std::vector<Entity*> _entityList;
	std::vector<ManagedResource*> _resourceList;

	void addToList(Entity* go);
};

#endif // SCENE_H
