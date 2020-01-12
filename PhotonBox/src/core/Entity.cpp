#include "PhotonBox/core/Entity.h"

#include <iostream>

#include "PhotonBox/resource/Scene.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

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
	std::vector<Transform*> children = transform->getChildren();
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->getEntity()->destroy();
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

	std::vector<Transform*> children = transform->getChildren();
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->getEntity()->setEnable(enable);
	}

	_isEnabled = enable;
}

void Entity::setStatic(bool _static)
{
	_isStatic = _static;
}