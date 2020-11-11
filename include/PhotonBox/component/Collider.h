#ifndef COLLIDER_H
#define COLLIDER_H

namespace physx
{
	class PxGeometry;
}

#include "PhotonBox/core/Component.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT Collider : public Component
{
public:
	virtual ~Collider() {}

	//void init() override;
	//void destroy() override;

	virtual physx::PxGeometry* getShape() = 0;
	std::type_index getBaseType() override;
};

#endif // COLLIDER_H
