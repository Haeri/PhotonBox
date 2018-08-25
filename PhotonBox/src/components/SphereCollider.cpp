#include "PhotonBox/components/SphereCollider.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

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
