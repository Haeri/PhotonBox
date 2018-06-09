#include <iostream>
#include "PhotonBox/core/Entity.h"

#include "PhotonBox/resources/Scene.h"

int Entity::_idCnt = 0;

Entity::Entity(Scene &_scene)
{
	Entity(_scene, "Entity");
}

Entity::Entity(Scene &_scene, std::string _name)
{
	_componentMap = std::unordered_map<std::type_index, Component*>();
	name = _name;
	_id = Entity::_idCnt++;
	_isEnabled = true;
	_isStatic = true;
	transform = addComponent<Transform>();
}

void Entity::destroyComponents()
{
	for (std::unordered_map<std::type_index, Component*>::iterator it = _componentMap.begin(); it != _componentMap.end();)
	{
		Component *c = _componentMap[(it->first)];
		c->destroy();
		it = _componentMap.erase(it);
		delete c;
	}
}

void Entity::destroy()
{

	if (!transform->children.empty())
	{
		for (std::vector<Transform*>::iterator it = transform->children.begin(); it != transform->children.end(); ++it)
		{
			(*it)->entity->destroy();
		}
	}

	destroyComponents();
	parentScene->removeFromList(this);
}

int Entity::getId()
{
	return _id;
}

void Entity::setEnable(bool enable)
{
	for (std::unordered_map<std::type_index, Component*>::const_iterator it = _componentMap.begin(); it != _componentMap.end(); ++it)
	{
		it->second->setEnable(enable);
	}

	for (std::vector<Transform*>::iterator it = transform->children.begin(); it != transform->children.end(); ++it)
	{
		(*it)->entity->setEnable(enable);
	}

	_isEnabled = enable;
}

void Entity::setStatic(bool _static)
{
	_isStatic = _static;
}

void Entity::printComponents()
{
	std::cout << name << " contains " << _componentMap.size() << ":\n<";
	for (std::unordered_map<std::type_index, Component*>::const_iterator it = _componentMap.begin(); it != _componentMap.end(); ++it)
	{
		if (it == _componentMap.begin())
			std::cout << it->second->getName();
		else
			std::cout << ", " << it->second->getName();
	}
	std::cout << ">" << std::endl;
}
