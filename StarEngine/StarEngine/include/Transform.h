#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include "Vector3f.h"
#include "Matrix4f.h"
#include "Component.h"

class Transform : public Component{
public:

	void setPosition(Vector3f position);
	void setRotation(Vector3f rotation);
	void setRotation(Matrix4f rot);
	void setScale(Vector3f scale);
	void setParent(Transform *_parent);
	void setParent(GameObject *_gameObject);
	Matrix4f getRotationMatrix();
	Matrix4f getRotation();
	Vector3f getPosition();
	Vector3f getScale();

	Matrix4f getTransformationMatrix();
	Vector3f forward();
	Vector3f up();
	std::vector<Transform*> children;
	Transform* getParent() { return _parent; }
	void removeChild(Transform* child);
private:
	Matrix4f _rotMat;
	bool _hasChanged = true;
	Vector3f _position = Vector3f::ZERO;
	//Vector3f _rotation = Vector3f::ZERO;
	Vector3f _scale = Vector3f::ONE;
	Matrix4f _cache;
	Transform* _parent;
	
	Matrix4f getLocalTransformationMatrix();
};
#endif /* defined(TRANSFORM_H) */
