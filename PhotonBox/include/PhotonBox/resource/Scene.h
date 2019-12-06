#ifndef SCENE_H
#define SCENE_H

class ManagedResource;

#include <vector>
#include <map>
#include <type_traits>

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
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		T *res = new T(data);
		_resourceMap[name] = res;

		return res;
	}
	template <class T>
	T* createResource(std::string name)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		T* res = new T();
		_resourceMap[name] = res;

		return res;
	}

	template <class T, typename C>
	T* createResource(Filepath path, C config)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		T* res = new T(path, config);
		_resourceMap[path.getAbsolutePath()] = res;

		return res;
	}
	template <class T>
	T* createResource(Filepath path)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		T* res = new T(path);
		_resourceMap[path.getAbsolutePath()] = res;

		return res;
	}

	void destroy(Entity* go);
	void removeFromList(Entity* go);
	Entity* getObjectByName(std::string name);
	std::vector<Entity*>& getEntities();
private:
	std::vector<Entity*> _entityList;
	std::map<std::string, ManagedResource*> _resourceMap;

	void addToList(Entity* go);
};

#endif // SCENE_H
