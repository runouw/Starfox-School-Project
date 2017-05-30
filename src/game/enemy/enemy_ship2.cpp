#include "enemies.h"

#include <rtools\ai\shaders.h>
#include <rtools\post\deferred.h>
#include <game\models\model_laser.h>
#include <game\engine.h>
#include <game\particle\particlebank.h>

Model* Enemy_Ship2::ship;
void Enemy_Ship2::init(){
	const struct aiScene* scene;

	Material::setDefaultProgram(Shaders::getDeferredShader());

	// to load textures from correct location:
	Material::setBasePath("../res/models/armada/");
	scene = aiImportFile("../res/models/armada/ship2.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene){
		// TODO: quit?
		printf("OBJ failed to load!");
	}

	// extract model from OBJ:
	ship = new Model(scene, scene->mRootNode);
	ship->init();
}

Enemy_Ship2::Enemy_Ship2(GsVec pos, GsVec dir) : dir(dir) {
	this->pos = pos;
	laserTimer = 0;
	shootingLaser = false;
	laserWait = 0;

	rz = 0;
	rzv = 0;
	exists = true;
}

void Enemy_Ship2::idle(float dt) {
	float vel = dt * 4.0f;
	pos += dir * vel;
	/*
	float t = Engine::getTimer();
	dir.x = cos(t);
	dir.z = sin(t);
	*/

	t += dt;
	if (Engine::player != NULL){
		Player* player = Engine::player;
		GsVec ppos = Engine::getPlayerPosition();

		if (pos.z > ppos.z + 5.0f){
			// TODO: for better enemy pruning, check if the enemy is past the near clipping plane of the camera (should
			// be be pretty good  even for scenes that rotate the camera)
			exists = false;
		}
	}

}
void Enemy_Ship2::draw(const GsMat& tr, const GsMat& pr) {
	GsMat scale, translate, rotX, rotY, rotZ;
	scale.scale(.03f, .03f, .03f);
	translate.translation(pos.x, pos.y, pos.z);

	float rx = atan2(dir.x, dir.z);
	float ry = atan2(dir.y, sqrt(dir.z * dir.z + dir.x * dir.x));
	rotZ.rotz(rz);
	rotX.roty(rx + AI_MATH_PI_F); // + AI_MATH_PI_F
	rotY.rotx(ry);

	ship->draw(scale * rotZ * rotY * rotX * translate * tr, pr);
}
void Enemy_Ship2::addDraw(const GsMat& tr, const GsMat& pr) {
	float t = Engine::getTimer();

}

void Enemy_Ship2::putLights(){
	DeferredUtils::addLight(Light(pos - dir * .2f, 5, RColor(0.9f, .1f, 0.1f, 1)));
}


bool Enemy_Ship2::projectileCollisionCheck(GsVec center, float radius, DamageData& data){
	if ((pos - center).len() < radius + .4f){
		data.onhit(data, this);

		// Draw a few particles
		for (int i = 0; i < 14; i++){
			GLTexture* tex;
			if (rand() % 3 == 0){
				tex = Textures::get("../res/images/bakuhatu01.bmp");
			}
			else if (rand() % 2 == 0){
				tex = Textures::get("../res/images/bakuhatu02.bmp");
			}
			else{
				tex = Textures::get("../res/images/bakuhatu03.bmp");
			}

			std::vector<Particle*>* list = ParticleUtils::getParticleList(Engine::particles, tex->texId);

			GsVec rndvel = GsVec((float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1));
			rndvel.normalize();

			rndvel *= (float)(gs_randomd()*gs_randomd() * 2.0f);
			rndvel.z += 1;

			float rndsize = (float)(gs_randomd() * 1.4f) + 1.2f;

			float rndlife = (float)(gs_randomd() * gs_randomd() * 0.6f) + 0.2f;

			Particle_ExplosionDecay* particle = (ParticleBank::explosionDecay[ParticleBank::explosionDecay_num++]);

			if (ParticleBank::explosionDecay_num >= PARTICLEBANK_NUM_EXPLOSIONDECAY){
				ParticleBank::explosionDecay_num = 0;
			}
			particle->exists = true;
			particle->del = false;
			particle->pos = pos;
			particle->pos.z += .5f;
			particle->initialSize = rndsize;
			particle->color = RColor(1, 1, 1, (float)gs_randomd());
			particle->vel = rndvel;
			particle->duration = rndlife;
			particle->t = 0;
			particle->rotSpeed = (float)(gs_randomd() * 2 - 1) * 4.0f;

			list->push_back(particle);
		}


		GLTexture* tex = Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp");

		std::vector<Particle*>* list = ParticleUtils::getParticleList(Engine::particles, tex->texId);

		for (int i = 0; i < 80; i++){
			GsVec rndvel = GsVec((float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1));
			rndvel.normalize();

			rndvel *= (float)(gs_randomd()*gs_randomd() * 4.0f);

			float rndsize = (float)(gs_randomd() * .3f) + .2f;

			float rndlife = (float)(gs_randomd() * gs_randomd() * 5.4f) + .1f;

			Particle_SimpleDecay* particle = (ParticleBank::simpleDecay[ParticleBank::simpleDecay_num++]);

			if (ParticleBank::simpleDecay_num >= PARTICLEBANK_NUM_SIMPLEDECAY){
				ParticleBank::simpleDecay_num = 0;
			}
			particle->exists = true;
			particle->del = false;
			particle->pos = pos;
			particle->initialSize = rndsize;
			particle->color = RColor(1, .5f, .2f, (float)gs_randomd());
			particle->vel = rndvel;
			particle->duration = rndlife;
			particle->t = 0;

			list->push_back(particle);
		}


		exists = false;
		return true;
	}
	return false;
}