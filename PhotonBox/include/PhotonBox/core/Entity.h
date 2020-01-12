#ifndef ENTITY_H
#define ENTITY_H

class Scene;

#include <unordered_map>
#include <typeindex>
#include <iostream>

#include "PhotonBox/component/Transform.h"
#include "PhotonBox/core/Component.h"
#include "PhotonBox/util/Logger.h"

class Entity
{
public:
	Scene * parentScene;
	Transform* transform;
	std::string name;

	Entity(Scene &_scene);
	Entity(Scene &_scene, std::string _name);

	template <class T>
	T* addComponent()
	{
		if (_componentMap.find(typeid(T)) != _componentMap.end())
		{
			Logger::errln(name, "Entity already contains Component");
			return nullptr;
		}
		T* c = new T();
		c->setup(this, transform);

		_componentMap.insert(std::pair<std::type_index, Component*>(typeid(T), c));
		return c;
	}

	template<class T>
	T* getComponent()
	{
		if (_componentMap.find(typeid(T)) == _componentMap.end())
		{
			for(std::unordered_map<std::type_index, Component*>::iterator it = _componentMap.begin(); it != _componentMap.end(); ++it)
			{
				if (typeid(T).hash_code() == it->second->getBaseType().hash_code())
				{
					return (T*)it->second;
				}
			}
			
			Logger::errln("Component", typeid(T).name(), "could not be found!");
			return nullptr;
		}
		return (T*)_componentMap[typeid(T)];
	}

	template <class T>
	void removeComponent()
	{
		T* c = getComponent<T>();
		_componentMap.erase(typeid(T));
		delete c;
	}

	void removeComponent(std::type_index ti)
	{
		Component *c = _componentMap[ti];
		c->destroy();
		_componentMap.erase(ti);
		delete c;
	}

	void destroyComponents();
	void destroy();
	int getId();
	void setEnable(bool enable);
	bool getEnable() { return _isEnabled; }
	void setStatic(bool _static);
	bool getStatic() { return _isStatic; }
private:
	static int _idCnt;
	bool _isEnabled;
	bool _isStatic;
	int _id;
	std::unordered_map<std::type_index, Component*> _componentMap;
};
#endif // ENTITY_H
