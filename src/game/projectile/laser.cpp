#include "projectiles.h"

#include <rtools\ai\model.h>
#include <rtools\ai\mesh.h>
#include <rtools\ai\material.h>
#include <rtools\utils.h>

#include <game\models\arwing.h>

#include <rtools\post\deferred.h>

#include <game\engine.h>

#include <game\particle\particlebank.h>

#include <game\models\model_laser.h>

#include <vector>

Laser::Laser(const GsVec p, const GsVec dir){
	pos = p;
	this->dir = dir;

	vel = 260.0f;

	life = 0;
	exists = true;
}

void onHit(const DamageData& data, const Enemy* enemy){
	Laser* laser = (Laser*)data.sender;
	laser->exists = false;
}

void Laser::idle(float dt){
	pos += dir * vel * dt;

	life += dt;

	DamageData data = DamageData(1, 0, dir * .1f, this, onHit);

	for (float d = 0; d < 1; d += .03f){
		for (size_t i = 0; i < Engine::enemies.size(); i++){
			if (Engine::enemies[i]->projectileCollisionCheck(pos + dir * d * 20.0f, 1, data)) break;
		}
	}

	float intersection = Engine::scene->intersect(pos, pos + dir * 20.0f);
	if (intersection >= 0){
		GLTexture* tex = Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp");

		std::vector<Particle*>* list = ParticleUtils::getParticleList(Engine::particles, tex->texId);

		for (int i = 0; i < 25; i++){
			GsVec rndvel = GsVec((float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1));
			rndvel.normalize();

			rndvel *= (float)(gs_randomd()*gs_randomd() * 2.0f);

			float rndsize = (float)(gs_randomd() * .3f) + .1f;

			float rndlife = (float)(gs_randomd() * gs_randomd() * 5.4f) + .2f;

			Particle_SimpleDecay* particle = (ParticleBank::simpleDecay[ParticleBank::simpleDecay_num++]);

			if (ParticleBank::simpleDecay_num >= PARTICLEBANK_NUM_SIMPLEDECAY){
				ParticleBank::simpleDecay_num = 0;
			}
			particle->exists = true;
			particle->del = false;
			particle->pos = pos + intersection*dir;
			particle->initialSize = rndsize;
			particle->color = RColor(.2f, .8f, .4f, (float)gs_randomd());
			particle->vel = rndvel;
			particle->duration = rndlife;
			particle->t = 0;

			list->push_back(particle);
		}

		exists = false;


		

	}

	if (life > 4){
		exists = false;
	}
}
void Laser::addDraw(const GsMat& tr, const GsMat& pr){
	Model_Laser::addDraw(tr, pr, RColor(0, 1, 0, 1), pos, dir * 15, .15f);
}

void Laser::putLights(){
	DeferredUtils::addLight(Light(pos + dir*1.0f, 10 + (10.0f / (life * 10.0f + 1.0f)), RColor(0.7f, 1, 0.7f, 1)));
}