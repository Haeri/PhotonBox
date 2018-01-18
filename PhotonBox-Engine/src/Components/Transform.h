#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include "../Math/Vector3f.h"
#include "../Math/Matrix4f.h"
#include "../Core/Component.h"

class Transform : public Component
{
public:
	void setPosition(Vector3f position);
	void setRotation(Vector3f rotation);
	void setScale(Vector3f scale);
	void setParent(Transform *_parent);
	void setParent(GameObject *_gameObject);
	Matrix4f getRotationMatrix();
	Vector3f getRotation();
	Vector3f getPosition();
	Vector3f getPositionWorld();
	Vector3f getScale();

	Matrix4f getTransformationMatrix();
	Matrix4f getTransformationMatrix(bool, bool, bool);
	Matrix4f getLocalTransformationMatrix();
	Matrix4f getLocalTransformationMatrix(bool, bool, bool);
	Vector3f forward();
	Vector3f forwardWorld();
	Vector3f up();
	Vector3f upWorld();
	Vector3f right();
	Vector3f rightWorld();
	std::vector<Transform*> children;
	Transform* getParent() { return _parent; }
	void removeChild(Transform* child);
	void renderHandels();
	void print();

private:
	bool _hasChanged = true;
	Vector3f _position = Vector3f::ZERO;
	Vector3f _rotation = Vector3f::ZERO;
	Vector3f _scale = Vector3f::ONE;
	Matrix4f _modelMatrixCached;
	Transform* _parent;
};
#endif // TRANSFORM_H
