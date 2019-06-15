#include "PhotonBox/core/system/ResourceManager.h"

#include <chrono>
#include <algorithm>

#include "PhotonBox/core/ILazyLoadable.h"
#include "PhotonBox/core/system/DebugGUI.h"
#include "PhotonBox/util/Logger.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

std::vector<ILazyLoadable*> ResourceManager::_initializationList;
unsigned int ResourceManager::max_loadtime = 300000;

bool ResourceManager::allReady()
{
	return _initializationList.empty();
}

void ResourceManager::lazyLoad(bool force)
{
	if (allReady()) return;
	
	ImGui::Begin("Assets Loader");
	for (int i = 0; i < _initializationList.size(); ++i)
	{
		ImGui::TextUnformatted((std::to_string(i+1) + " " + _initializationList[i]->getFilePath()).c_str());
	}
	ImGui::End();

	auto start = std::chrono::system_clock::now();
	std::vector<IndexedLazy> _readyList;

	while (!_initializationList.empty())
	{		
		_readyList.clear();
		for (int i = 0; i < _initializationList.size(); ++i)
		{
			if (_initializationList[i]->isLoaded())
			{
				_readyList.push_back({ i, _initializationList[i] });
			}
		}

		for (int i = static_cast<int>(_readyList.size() - 1); i >= 0; --i)
		{			
			_readyList[i].resource->initialize();
			_initializationList.erase(_initializationList.begin() + _readyList[i].index);

			auto check = std::chrono::system_clock::now();
			if ((check - start).count() > max_loadtime && !force) return;
		}

		if (!force) return;
	}
}

void ResourceManager::addToInitializationList(ILazyLoadable * resource)
{
	_initializationList.push_back(resource);
}

void ResourceManager::reset()
{
	_initializationList.clear();
}

