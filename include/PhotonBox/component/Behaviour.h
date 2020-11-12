#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "PhotonBox/core/Component.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT Behaviour : public Component
{
public:
	virtual ~Behaviour() {}

	void init() override;
	void destroy();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void OnDestroy();
};

#endif // BEHAVIOUR_H
