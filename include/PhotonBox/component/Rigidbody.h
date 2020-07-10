#ifndef RIGIDBODY_H
#define RIGIDBODY_H

class PxRigidBody;

#include "PhotonBox/core/Component.h"
#include "PhotonBox/math/Vector3f.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT Rigidbody : public Component
{
public:
	~Rigidbody();

	void init() override;
	physx::PxRigidBody* getBody();
	void setMass(float density);
	float getMass();
	void addForce(Vector3f direction);
private:
	bool _isKinematic = false;

	physx::PxRigidBody* _body;
};

#endif // RIGIDBODY_H
