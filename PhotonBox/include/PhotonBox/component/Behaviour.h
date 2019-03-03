#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "PhotonBox/core/Component.h"

class Behaviour : public Component
{
public:
	Behaviour();
	virtual ~Behaviour() {}

	void destroy();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void OnDestroy();
};

#endif // BEHAVIOUR_H
