#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <iostream>
#include "Behaviour.h"

class ParticleSystem : public Behaviour {
public:
	void Start() {
		std::cout << "LETSE STARTE" << std::endl;
	}

	void sayHi() {
		std::cout << "Hi!!!!!!!!!!!!!!!!" << std::endl;
	}
};

#endif /* defined(PARTICLE_SYSTEM_H) */