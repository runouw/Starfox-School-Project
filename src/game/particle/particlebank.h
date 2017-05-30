#pragma once

#include "particles.h"

#define PARTICLEBANK_NUM_SIMPLEDECAY 100000
#define PARTICLEBANK_NUM_EXPLOSIONDECAY 100000

class ParticleBank{
public:
	static Particle_SimpleDecay** simpleDecay;
	static int simpleDecay_num;

	static Particle_ExplosionDecay** explosionDecay;
	static int explosionDecay_num;

	static void allocate(){
		simpleDecay = new Particle_SimpleDecay*[PARTICLEBANK_NUM_SIMPLEDECAY];
		for (int i = 0; i < PARTICLEBANK_NUM_SIMPLEDECAY; i++){
			simpleDecay[i] = new Particle_SimpleDecay();
			simpleDecay[i]->del = false;
			simpleDecay[i]->exists = true;
		}
		simpleDecay_num = 0;

		explosionDecay = new Particle_ExplosionDecay*[PARTICLEBANK_NUM_EXPLOSIONDECAY];
		for (int i = 0; i < PARTICLEBANK_NUM_EXPLOSIONDECAY; i++){
			explosionDecay[i] = new Particle_ExplosionDecay();
			explosionDecay[i]->del = false;
			explosionDecay[i]->exists = true;
		}
		explosionDecay_num = 0;
	}

};