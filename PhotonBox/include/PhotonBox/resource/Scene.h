#ifndef SCENE_H
#define SCENE_H

class ManagedResource;

#include <vector>
#include <map>

#include "PhotonBox/core/Entity.h"
#include "PhotonBox/resource/CubeMap.h"
#include "PhotonBox/resource/Mesh.h"
#include "PhotonBox/resource/Texture.h"
#include "PhotonBox/resource/Filepath.h"

class Scene
{
public:
	virtual void Load() = 0;
	virtual void OnUnload();
	void unload();

	Entity* instantiate(std::string name);

	template <class T, typename D>
	T* createResource(std::string name, D data) 
	{
		T *res = new T(data);
		_resourceMap[name] = res;

		return res;
	}
	template <class T>
	T* createResource(std::string name)
	{
		T* res = new T();
		_resourceMap[name] = res;

		return res;
	}

	template <class T, typename C>
	T* createResource(Filepath path, C config)
	{
		T* res = new T(path, config);
		_resourceMap[path.getAbsolutePath()] = res;

		return res;
	}
	template <class T>
	T* createResource(Filepath path)
	{
		T* res = new T(path);
		_resourceMap[path.getAbsolutePath()] = res;

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
	//std::vector<ManagedResource*> _resourceList;
	std::map<std::string, ManagedResource*> _resourceMap;

	void addToList(Entity* go);
};

#endif // SCENE_H
