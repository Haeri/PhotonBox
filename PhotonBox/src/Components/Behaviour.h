#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "../Core/Component.h"

class Behaviour : public Component
{
public:
	Behaviour();
	void destroy();

	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void OnDestroy();
};

#endif /* defined(BEHAVIOUR_H) */
