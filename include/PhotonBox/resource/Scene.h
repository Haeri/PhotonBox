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
#include "PhotonBox/core/system/ResourceManager.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT Scene
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

		_resources.push_back(name);

		return ResourceManager::createResource<T, D>(name, data);
	}

	template <class T>
	T* createResource(std::string name)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		_resources.push_back(name);

		return ResourceManager::createResource<T>(name);
	}

	template <class T, typename C>
	T* createResource(Filepath path, C config)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		_resources.push_back(path.getAbsolutePath());

		return ResourceManager::createResource<T, C>(path, config);
	}
	template <class T>
	T* createResource(Filepath path)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		_resources.push_back(path.getAbsolutePath());

		return ResourceManager::createResource<T>(path);
	}

	void destroy(Entity* go);
	void removeFromList(Entity* go);
	Entity* getObjectByName(std::string name);
	std::vector<Entity*>& getEntities();
private:
	std::vector<Entity*> _entityList;
	std::vector<std::string> _resources;

	void addToList(Entity* go);
};

#endif // SCENE_H
