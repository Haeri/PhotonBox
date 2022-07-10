#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class LazyLoadable;
class ManagedResource;

#include <vector>
#include <map>

#include "PhotonBox/resource/Filepath.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT ResourceManager
{
public:
	static unsigned int max_loadtime;

	void load(bool lazy = true);
	void reset();
	void destroy();

	template <class T, typename D>
	static T* createResource(std::string name, D data)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		T* resource;

		if (_resourceMap.find(name) == _resourceMap.end())
		{
			resource = new T(data);
			_resourceMap[name] = { 1, resource };
		}
		else
		{
			CountedResource* cr = &_resourceMap[name];
			++cr->count;
			resource = (T*)cr->managedResource;
		}

		return resource;
	}
	template <class T>
	static T* createResource(std::string name)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		T* resource;

		if (_resourceMap.find(name) == _resourceMap.end())
		{
			resource = new T();
			_resourceMap[name] = { 1, resource };
		}
		else
		{
			CountedResource* cr = &_resourceMap[name];
			++cr->count;
			resource = (T*)cr->managedResource;
		}

		return resource;
	}

	template <class T, typename C>
	static T* createResource(Filepath path, C config)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		T* resource;

		if (_resourceMap.find(path.getFilePath()) == _resourceMap.end())
		{
			resource = new T(path, config);
			_resourceMap[path.getFilePath()] = { 1, resource };
		}
		else
		{
			CountedResource* cr = &_resourceMap[path.getFilePath()];
			++cr->count;
			resource = (T*)cr->managedResource;
		}

		return resource;
	}
	template <class T>
	static T* createResource(Filepath path)
	{
		static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

		T* resource;

		if (_resourceMap.find(path.getFilePath()) == _resourceMap.end())
		{
			resource = new T(path);
			_resourceMap[path.getFilePath()] = { 1, resource };
		}
		else
		{
			CountedResource* cr = &_resourceMap[path.getFilePath()];
			++cr->count;
			resource = (T*)cr->managedResource;
		}

		return resource;
	}

	static void deleteResource(Filepath path);
	static void deleteResource(std::string name);

	static bool isCompleted();
	static void addToInitializationList(LazyLoadable* resource);
private:
	struct CountedResource
	{
		int count;
		ManagedResource* managedResource;
	};

	static std::map<std::string, CountedResource> _resourceMap;
	static std::vector<LazyLoadable*> _initializationList;
	static std::vector<LazyLoadable*> _readyList;
};

#endif // RESOURCE_MANAGER_H
