#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;
class Transform;

#include <string>
#include <typeindex>

class Component
{
public:
	Entity* entity;
	Transform* transform;

	void setEntity(Entity* _go);
	void setTransform(Transform* _t);
	void setEnable(bool enable);
	std::string getName();
	bool getEnable();

	virtual ~Component() = 0;
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void destroy();
	virtual std::type_index getBaseType();
private:
	bool _isEnabled = true;
};
#endif // COMPONENT_H
