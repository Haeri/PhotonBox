#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class Scene;

#include <unordered_map>
#include <typeindex>
#include <iostream>
#include "Component.h"
#include "../Components/Transform.h"

class GameObject {
public:
	Scene* parentScene;
	Transform* transform;
	std::string name;

	GameObject(Scene &_scene);
	GameObject(Scene &_scene, std::string _name);
	
	template <class T>
	T* addComponent() {
		if (componentMap.find(typeid(T)) != componentMap.end()) {
			std::cerr << name << " GameObject already contains Component " << std::endl;
			return nullptr;
		}
		T* c = new T();
		c->setGameObject(this);
		c->setTransform(transform);
		componentMap.insert(std::pair<std::type_index, Component*>(typeid(T), c));
		return c;
	}

	template<class T>
	T* getComponent(){
		if (componentMap.find(typeid(T)) == componentMap.end()) {
			std::cerr << "Component was not found!" << std::endl;
			return nullptr;
		}
		return (T*)componentMap[typeid(T)];
	}

	template <class T>
	void removeComponent() {
		T* c = getComponent<T>();
		componentMap.erase(typeid(T));
		delete c;
	}

	void removeComponent(std::type_index ti) {
		Component *c = componentMap[ti];
		c->destroy();
		componentMap.erase(ti);
		delete c;
	}

	void destroyComponents();
	void destroy();
	int getId();
	void setEnable(bool enable);
	bool getEnable() { return _isEnabled; }
	void printComponents();
private:
	static int _idCnt;
	bool _isEnabled;
	int _id;
	std::unordered_map<std::type_index, Component*> componentMap;
};
#endif /* defined(GAME_OBJECT_H) */
