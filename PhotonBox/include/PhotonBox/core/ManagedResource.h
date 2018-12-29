#ifndef MANAGED_RESOURCE_H
#define MANAGED_RESOURCE_H

#include <vector>

class ManagedResource
{
public:
	// PLAN:
	// Make a ResoueceManager class.
	// Every scene has a reference to a ResourceManager.
	// Creating a resource stores the path(as hash) in
	// a local set and a global map. Map has path as key and
	// a struct of pointer and counter as value. 
	// When unloading a scene, first load the new scene, query 
	// in the global map if the resoure already exists, increment 
	// counter and return the poointer. At the end delete all 
	// resources from the global map according to the local set.

	virtual ~ManagedResource() {}

	virtual void asyncLoad() = 0;
	virtual void sendToGPU() = 0;

	static void lazyLoad(bool force = false);
	static bool allReady();
protected:
	static std::vector<ManagedResource*> _initializationList;
	bool _isInitialized = false;
	bool _isLoaded = false;
};

#endif // MANAGED_RESOURCE_H
