#include "PhotonBox/component/SphereCollider.h"
#include "PhotonBox/core/system/Physics.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void SphereCollider::init()
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
	bool ret = _geometry->isValid();
}
