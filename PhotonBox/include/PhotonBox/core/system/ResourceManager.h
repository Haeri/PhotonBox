#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class LazyLoadable;

#include <vector>

class ResourceManager
{
public:
	static unsigned int max_loadtime;

	void load(bool lazy = true);
	void reset();

	static bool isCompleted();
	static void addToInitializationList(LazyLoadable* resource);
private:
	struct IndexedLazy
	{
		int index;
		LazyLoadable* resource;
	};
	static std::vector<LazyLoadable*> _initializationList;
};

#endif // RESOURCE_MANAGER_H
