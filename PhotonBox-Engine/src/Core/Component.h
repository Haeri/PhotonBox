#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Transform;
#include <string>

class Component
{
public:
	GameObject * gameObject;
	Transform *transform;

	void setGameObject(GameObject* _go);
	void setTransform(Transform* _t);
	void setEnable(bool enable);
	std::string getName();
	bool getEnable() { return isEnabled; }

	virtual ~Component() = 0;
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void destroy();
private:
	bool isEnabled = true;
};
#endif // COMPONENT_H