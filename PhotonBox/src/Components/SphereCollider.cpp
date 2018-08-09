#include "PhotonBox/components/SphereCollider.h"

SphereCollider::SphereCollider():Collider()
{
	_geometry = new physx::PxSphereGeometry(_radius);
}

SphereCollider::~SphereCollider()
{
	delete _geometry;
}

physx::PxGeometry* SphereCollider::getShape()
{
	return _geometry;
}

void SphereCollider::setRadius(float r)
{
	_radius = r;
	_geometry->radius = r;
}
