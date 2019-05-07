#include "PhotonBox/data-type/AABB.h"

#include "PhotonBox/component/Transform.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void AABB::setTransform(Transform* transform)
{
	_transform = transform;
}

void AABB::setMinBound(Vector3f minBound)
{
	_minBound = minBound;
}

void AABB::setMaxBound(Vector3f maxBound)
{
	_maxBound = maxBound;
}

Vector3f AABB::getMinBoundGlobal()
{
	return _transform->getPositionWorld() + _minBound;
}

Vector3f AABB::getMaxBoundGlobal()
{
	return _transform->getPositionWorld() + _maxBound;
}

Vector3f AABB::getBoundPosition()
{
	return _transform->getPositionWorld();
}

bool AABB::collide(Vector3f point)
{
	Vector3f min = getMinBoundGlobal();
	Vector3f max = getMaxBoundGlobal();

	return (point.x() > min.x() && point.x() < max.x() &&
			point.x() > min.x() && point.x() < max.x() &&
			point.x() > min.x() && point.x() < max.x());
}
