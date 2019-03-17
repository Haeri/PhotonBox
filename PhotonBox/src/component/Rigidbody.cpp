#include "PhotonBox/component/Rigidbody.h"

#include "PhotonBox/core/system/Physics.h"
#include "PhotonBox/core/Entity.h"
#include "PhotonBox/component/Collider.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void Rigidbody::init()
{
	PxPhysics*	physics = Physics::getPhysX();
	Matrix4f mat = transform->getTransformationMatrix();
	PxMaterial*_gMaterial = physics->createMaterial(0.5f, 0.5f, 0.6f);

	const PxMat44 pmat(mat.getArray());
	Collider* c = entity->getComponent<Collider>();
	if (c == nullptr)
	{
#ifdef _DEBUG
		std::cerr << entity->name << "-Entity with a rigidbody requires a Collider component!\n";
#endif
		return;
	}

	PxGeometry* geo = c->getShape();
	PxTransform t(pmat);
	PxRigidDynamic* dynamic = PxCreateDynamic(*physics, t, *geo, *_gMaterial, 1);
	dynamic->userData = entity;
	_body = dynamic;
	
	Physics::registerObject(this);
}

Rigidbody::~Rigidbody()
{
	Physics::removePhysicsObject(this);
}

physx::PxRigidBody* Rigidbody::getBody()
{
	return _body;
}

float Rigidbody::getMass()
{
	return _body->getMass();
}

void Rigidbody::setMass(float mass)
{
	_body->setMass(mass);
}

void Rigidbody::addForce(Vector3f direction)
{
	_body->addForce(PxVec3(direction.getX(), direction.getY(), direction.getZ()));
}
