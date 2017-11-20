#include "AABB.h"
#include "../Components/Transform.h"

void AABB::setTransform(Transform * transform){
	_transform = transform;
}

void AABB::setMinBound(Vector3f minBound){
	_minBound = minBound;
}

void AABB::setMaxBound(Vector3f maxBound){
	_maxBound = maxBound;
}

Vector3f AABB::getMinBoundGlobal(){
	return _transform->getPositionWorld() + _minBound;
}

Vector3f AABB::getMaxBoundGlobal(){
	return _transform->getPositionWorld() + _maxBound;
}

Vector3f AABB::getBoundPosition(){
	return _transform->getPositionWorld();
}

bool AABB::collide(Vector3f other){
	Vector3f min = getMinBoundGlobal();
	Vector3f max = getMaxBoundGlobal();

	return (other.x() > min.x() && other.x() < max.x() &&
			other.x() > min.x() && other.x() < max.x() &&
			other.x() > min.x() && other.x() < max.x());
}
