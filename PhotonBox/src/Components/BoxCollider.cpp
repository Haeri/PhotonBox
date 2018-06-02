#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
	_geometry = new physx::PxBoxGeometry();
}

BoxCollider::~BoxCollider()
{
	delete _geometry;
}

physx::PxGeometry * BoxCollider::getShape()
{
	return _geometry;
}

void BoxCollider::setHalfExtents(Vector3f extents)
{
	_extents = extents;
	_geometry->halfExtents = extents.toPhysX();
}
