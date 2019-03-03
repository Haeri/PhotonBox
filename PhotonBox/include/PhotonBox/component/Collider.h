#ifndef COLLIDER_H
#define COLLIDER_H

namespace physx
{
	class PxGeometry;
}

#include "PhotonBox/core/Component.h"

class Collider : public Component
{
public:
	Collider();
	virtual ~Collider() {}

	void destroy();

	virtual physx::PxGeometry* getShape() = 0;
	std::type_index getBaseType() override { return typeid(Collider); }
};

#endif // COLLIDER_H
