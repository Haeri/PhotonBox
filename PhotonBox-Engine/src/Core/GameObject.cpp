#include <iostream>

#include "GameObject.h"
#include "../Resources/Scene.h"

int GameObject::_idCnt = 0;

GameObject::GameObject(Scene &_scene) {
	GameObject(_scene, "GameObject");
}

GameObject::GameObject(Scene &_scene, std::string _name) {
	componentMap = std::unordered_map<std::type_index, Component*>();
	name = _name;
	_id = GameObject::_idCnt++;
	_isEnabled = true;
	_isStatic = false;
	transform = addComponent<Transform>();
}

void GameObject::destroyComponents(){
	for (std::unordered_map<std::type_index, Component*>::iterator it = componentMap.begin(); it != componentMap.end();) {
		Component *c = componentMap[(it->first)];
		c->destroy();
		it = componentMap.erase(it);
		delete c;
	}
}

void GameObject::destroy() {

	if (!transform->children.empty()) {
		for (std::vector<Transform*>::iterator it = transform->children.begin(); it != transform->children.end(); ++it) {
			(*it)->gameObject->destroy();
		}
	}

	destroyComponents();
	parentScene->removeFromList(this);
}

int GameObject::getId() {
	return _id;
}

void GameObject::setEnable(bool enable) {
	for (std::unordered_map<std::type_index, Component*>::const_iterator it = componentMap.begin(); it != componentMap.end(); ++it){
		it->second->setEnable(enable);
	}

	for (std::vector<Transform*>::iterator it = transform->children.begin(); it != transform->children.end(); ++it) {
		(*it)->gameObject->setEnable(enable);
	}

	_isEnabled = enable;
}

void GameObject::setStatic(bool _static) {
	_isStatic = _static;
}

void GameObject::printComponents()
{
	std::cout << name << " contains " << componentMap.size() << ":\n<";
	for (std::unordered_map<std::type_index, Component*>::const_iterator it = componentMap.begin(); it != componentMap.end(); ++it)
	{
		if(it == componentMap.begin())
			std::cout << it->second->getName();
		else
			std::cout << ", " << it->second->getName();
	}
	std::cout << ">" << std::endl;
}
