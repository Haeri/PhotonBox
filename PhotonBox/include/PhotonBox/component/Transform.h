#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include "PhotonBox/core/Component.h"
#include "PhotonBox/math/Matrix4f.h"
#include "PhotonBox/math/Vector3f.h"
#include "PhotonBox/math/Quaternion.h"

class Transform : public Component
{
public:
	void setPosition(Vector3f position);
	void setRotation(Vector3f rotation);
	void setRotation(Quaternion rotation);
	void setScale(Vector3f scale);
	void setParent(Transform *_parent);
	void setParent(Entity *_entity);
	void rotate(Quaternion quat);
	Matrix4f getRotationMatrix();
	Quaternion getRotation();
	Vector3f getPosition();
	Vector3f getPositionWorld();
	Vector3f getScale();

	Matrix4f getTransformationMatrix();
	Matrix4f getLocalTransformationMatrix();
	Vector3f forward();
	Vector3f forwardWorld();
	Vector3f up();
	Vector3f upWorld();
	Vector3f right();
	Vector3f rightWorld();
	bool hasChangedSince(unsigned long int tick);
	bool hasChangedLastTick();
	Transform* getParent();
	std::vector<Transform*> getChildren();
	void renderHandels();
private:
	unsigned long int _lastChangedTick = -1;
	bool _isMatrixCached = false;
	Vector3f _position = Vector3f::ZERO;
	Quaternion _rotation = Quaternion::ZERO;
	Vector3f _scale = Vector3f::ONE;
	Matrix4f _transformationMatrixCached;
	Matrix4f _transformationMatrixLast;
	Transform* _parent;
	std::vector<Transform*> _children;

	void removeChild(Transform* child);
	void setChanged();
};
#endif // TRANSFORM_H
