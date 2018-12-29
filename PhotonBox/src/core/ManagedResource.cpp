#include "PhotonBox/core/ManagedResource.h"

#include <chrono>

std::vector<ManagedResource*> ManagedResource::_initializationList;

bool ManagedResource::allReady()
{
	return _initializationList.size() == 0;
}

void ManagedResource::lazyLoad(bool force)
{
	if (allReady()) { 
		return;
	}

	auto start = std::chrono::system_clock::now();
	for (int i = _initializationList.size()-1; i >= 0; --i)
	{
		if (_initializationList[i]->_isLoaded) {
			_initializationList[i]->sendToGPU();

			_initializationList.erase(_initializationList.begin() + i);

			auto check = std::chrono::system_clock::now();
			if((check - start).count() > 30000000 && !force) return;
		}
	}
}
