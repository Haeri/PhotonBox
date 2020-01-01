#include "PhotonBox/core/system/ResourceManager.h"

#include <chrono>
#include <algorithm>
#include <string>

#include "PhotonBox/core/LazyLoadable.h"
#include "PhotonBox/core/system/DebugGUI.h"
#include "PhotonBox/util/Logger.h"
#include "PhotonBox/core/ManagedResource.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

std::map<std::string, ResourceManager::CountedResource> ResourceManager::_resourceMap;
std::vector<LazyLoadable*> ResourceManager::_initializationList;
std::vector<LazyLoadable*> ResourceManager::_readyList;
unsigned int ResourceManager::max_loadtime = 300000;

bool ResourceManager::isCompleted()
{
	return _initializationList.empty() && _readyList.empty();
}

void ResourceManager::load(bool lazy)
{
	if (isCompleted()) return;
	
	ImGui::Begin("Assets Loader");
	ImGui::Text(("Initializing " + std::to_string(_readyList.size())).c_str());
	for (int i = 0; i < _readyList.size(); ++i)
	{
		ImGui::TextUnformatted((std::to_string(i+1) + " " + _readyList[i]->getFilePath()).c_str());
	}
	ImGui::Separator();
	ImGui::Text(("Loading " + std::to_string(_initializationList.size())).c_str());
	for (int i = 0; i < _initializationList.size(); ++i)
	{
		ImGui::TextUnformatted((std::to_string(i + 1) + " " + _initializationList[i]->getFilePath()).c_str());
	}
	ImGui::End();

	auto start = std::chrono::system_clock::now();

	while (!isCompleted())
	{		
		if(_readyList.empty())
		{
			_readyList.clear();
			for (int i = _initializationList.size() - 1; i >= 0; --i)
			{
				if (_initializationList[i]->getState() == LazyLoadable::State::LOADED ||
					_initializationList[i]->getState() == LazyLoadable::State::FAILED)
				{
					if (_initializationList[i]->getState() == LazyLoadable::State::LOADED)
					{
						_readyList.push_back(_initializationList[i]);
					}
					_initializationList.erase(_initializationList.begin() + i);
				}
			}
		}

		for (int i = _readyList.size() - 1; i >= 0; --i)
		{	
			_readyList[i]->initialize();
			_readyList.erase(_readyList.begin() + i);

			auto check = std::chrono::system_clock::now();
			if ((check - start).count() > max_loadtime && lazy) return;
		}

		if (lazy) return;
	}
}

void ResourceManager::addToInitializationList(LazyLoadable * resource)
{
	_initializationList.push_back(resource);
}

void ResourceManager::reset()
{
	_initializationList.clear();
}

void ResourceManager::destroy()
{
	for (std::map<std::string, CountedResource>::iterator it = _resourceMap.begin(); it != _resourceMap.end(); ++it)
	{
		LazyLoadable* f = dynamic_cast<LazyLoadable*>(it->second.managedResource);
		if (f != nullptr) {
			if (f->getState() == LazyLoadable::State::LOADING)
			{
				f->setRogue();
			}
			else
			{
				delete (it->second.managedResource);
			}
		}
		else
		{
			delete (it->second.managedResource);
		}

	}
	_resourceMap.clear();
}

void ResourceManager::deleteResource(Filepath path)
{
	deleteResource(path.getAbsolutePath());
}

void ResourceManager::deleteResource(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end())
	{
		CountedResource* cr = &_resourceMap[name];
		if (cr->count <= 1)
		{
			LazyLoadable* f = dynamic_cast<LazyLoadable*>(cr->managedResource);
			if (f != nullptr) {
				if (f->getState() == LazyLoadable::State::LOADING)
				{
					f->setRogue();
				}
				else
				{
					delete (cr->managedResource);
				}
			}
			else
			{
				delete (cr->managedResource);
			}
			_resourceMap.erase(name);
		}
		else
		{
			--cr->count;
		}
	}
}
