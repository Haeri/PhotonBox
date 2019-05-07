#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;
class Transform;

#include <string>
#include <typeindex>

class Component
{
public:
	void setEnable(bool enable);
	std::string getName();
	bool getEnable();
	Transform* getTransform();
	Entity* getEntity();

	void setup(Entity* entity, Transform* transform)
	{
		this->entity = entity;
		this->transform = transform;
		init();
	}

	virtual ~Component() = 0;
	virtual void init() {}
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void destroy();
	virtual std::type_index getBaseType();
protected:
	Entity* entity;
	Transform* transform;

private:
	bool _isEnabled = true;
};
#endif // COMPONENT_H
