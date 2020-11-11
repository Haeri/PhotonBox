#ifndef PB_BOX_COLLIDER_IMPL_H
#define PB_BOX_COLLIDER_IMPL_H

#include "PhotonBox/component/BoxCollider.h"

class BoxColliderImpl : public Collider
{
public:
	~BoxCollider();

	physx::PxGeometry* getShape() override;
	Vector3f _extents;
	physx::PxBoxGeometry* _geometry;
};

#endif // PB_BOX_COLLIDER_IMPL_H