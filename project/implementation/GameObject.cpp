#include "GameObject.h"
#include "Scene.h"

int GameObject::idCnt = 0;

GameObject::GameObject(Scene &_scene) {
	GameObject(_scene, "GameObject");
}

GameObject::GameObject(Scene &_scene, std::string _name) {
	componentMap = std::unordered_map<std::type_index, Component*>();
	name = _name;
	_id = GameObject::idCnt++;
	transform = addComponent<Transform>();
}

void GameObject::destroy() {
	for (std::unordered_map<std::type_index, Component*>::iterator it = componentMap.begin(); it != componentMap.end();) {
		Component *c = componentMap[(it->first)];
		c->destroy();
		it = componentMap.erase(it);
		delete c;
	}
}

int GameObject::getId() {
	return _id;
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