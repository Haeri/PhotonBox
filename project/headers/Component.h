#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Transform;
#include <string>

class Component {
public:
	GameObject *gameObject;
	Transform *transform;

	virtual ~Component() = 0;
	void setGameObject(GameObject* _go);
	void setTransform(Transform* _t);
	std::string getName();
	void destroy();
protected:
	virtual void Destroy();
};
#endif /* defined(COMPONENT_H) */
