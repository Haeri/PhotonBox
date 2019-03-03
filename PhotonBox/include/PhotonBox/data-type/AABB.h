#ifndef AABB_H
#define AABB_H

class Transform;

#include "PhotonBox/math/Vector3f.h"

class AABB
{
public:
	AABB() {}
	AABB(Vector3f minBound, Vector3f maxBound, Transform* transform) 
		: _minBound(minBound), _maxBound(maxBound), _transform(transform) {}
	void setTransform(Transform* transform);
	void setMinBound(Vector3f minBound);
	void setMaxBound(Vector3f paxBound);
	Vector3f getMinBound() { return _minBound; }
	Vector3f getMaxBound() { return _maxBound; }
	Vector3f getMinBoundGlobal();
	Vector3f getMaxBoundGlobal();
	Vector3f getBoundPosition();
	bool collide(Vector3f point);
private:
	Vector3f _minBound, _maxBound;
	Transform* _transform;
};

#endif // AABB_H
