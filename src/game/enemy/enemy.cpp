#include "enemies.h"
#include <game\engine.h>
#include <rtools\post\deferred.h>
#include <game\models\model_laser.h>
#include <game\particle\particlebank.h>

void Enemy::doLaser(const GsVec& pos, const GsVec& dir, float dis){
	float _dis = dis;
	float intersection = Engine::scene->intersect(pos, pos + dir * dis);
	if (intersection >= 0 && intersection < _dis){
		_dis = intersection; // shorten laser because of collision with scene
	}

	// check if laser collision with player
	intersection = Engine::player->lineIntersect(pos, pos + dir * _dis);
	if (intersection >= 0){
		_dis = intersection;
		//printf("Laser hit player %f!\n", intersection);


		// ADD LASER COLLISION PARTICLES AT COLLISION POINT
		GLTexture* tex = Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp");

		std::vector<Particle*>* list = ParticleUtils::getParticleList(Engine::particles, tex->texId);

		for (int i = 0; i < 10; i++){
			GsVec rndvel = GsVec((float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1));
			rndvel.normalize();

			rndvel *= (float)(gs_randomd()*gs_randomd() * 4.0f);

			float rndsize = (float)(gs_randomd() * .15f) + .05f;

			float rndlife = (float)(gs_randomd() * gs_randomd() * 2.4f) + .1f;

			Particle_SimpleDecay* particle = (ParticleBank::simpleDecay[ParticleBank::simpleDecay_num++]);

			if (ParticleBank::simpleDecay_num >= PARTICLEBANK_NUM_SIMPLEDECAY){
				ParticleBank::simpleDecay_num = 0;
			}
			particle->exists = true;
			particle->del = false;
			particle->pos = pos + dir * _dis;
			particle->initialSize = rndsize;
			particle->color = RColor(1, .2f, .1f, (float)gs_randomd());
			particle->vel = rndvel;
			particle->duration = rndlife;
			particle->t = 0;

			list->push_back(particle);
		}


		// END ADDING PARTICLES
	}
}

void Enemy::doLaserHintDraw(const GsVec& pos, const GsVec& dir, float dis, float str, const GsMat& tr, const GsMat& pr){
	float _dis = dis;
	float intersection = Engine::scene->intersect(pos, pos + dir * dis);
	if (intersection >= 0){
		_dis = intersection; // shorten laser because of collision with scene
	}

	Model_Laser::addDraw(tr, pr, RColor(.2f*str, .2f*str, .2f*str, 1), pos, dir * _dis, .1f * str);
}
void Enemy::doLaserDraw(const GsVec& pos, const GsVec& dir, float dis, float str, const GsMat& tr, const GsMat& pr){
	float _dis = dis;
	float intersection = Engine::scene->intersect(pos, pos + dir * dis);
	if (intersection >= 0 && intersection < _dis){
		_dis = intersection; // shorten laser because of collision with scene
	}

	Model_Laser::addDraw(tr, pr, RColor(1 * str, .2f * str, .2f * str, 1), pos, dir * _dis, .15f * str);
}

void Enemy::doLaserLights(const GsVec& pos, const GsVec& dir, float dis){
	float _dis = dis;
	float intersection = Engine::scene->intersect(pos, pos + dir * dis);
	if (intersection >= 0 && intersection < _dis){
		_dis = intersection; // shorten laser because of collision with scene
	}

	float dd = (_dis / dis);
	if (dd > 1) dd = 1;
	if (dd < 0) dd = 0;
	for (float t = 0; t < dd; t += .05f){
		DeferredUtils::addLight(Light(pos + dir * t * _dis, 5, RColor(0.9f, .1f, 0.1f, .5f)));
	}

}