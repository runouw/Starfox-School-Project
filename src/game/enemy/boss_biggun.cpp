#include "enemies.h"

#include <rtools\ai\shaders.h>
#include <rtools\post\deferred.h>
#include <game\models\model_laser.h>
#include <game\engine.h>
#include <game\particle\particlebank.h>

#define BOSS_SCALE .02f

Model* Boss_BigGun::ship;
Model* Boss_BigGun::shipLights;
Model* Boss_BigGun::button;
void Boss_BigGun::init(){
	const struct aiScene* scene;
	{
		Material::setDefaultProgram(Shaders::getDeferredShader());

		// to load textures from correct location:
		Material::setBasePath("../res/models/armada/");
		scene = aiImportFile("../res/models/armada/redgunship1.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!");
		}

		// extract model from OBJ:
		ship = new Model(scene, scene->mRootNode);
		ship->init();
	}
	{
		Material::setDefaultProgram(Shaders::getDeferredShader());

		// to load textures from correct location:
		Material::setBasePath("../res/models/armada/");
		scene = aiImportFile("../res/models/armada/redgunship1_lights.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!");
		}

		// extract model from OBJ:
		shipLights = new Model(scene, scene->mRootNode);
		shipLights->init();
	}
	{
		Material::setDefaultProgram(Shaders::getDeferredShader());

		// to load textures from correct location:
		Material::setBasePath("../res/models/other/");
		scene = aiImportFile("../res/models/other/bigredbutton.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!");
		}

		// extract model from OBJ:
		button = new Model(scene, scene->mRootNode);
		button->init();
	}
}

Boss_BigGun::Boss_BigGun(GsVec pos, GsVec dir) : dir(dir) {
	this->pos = pos;
	laserTimer = 0;
	laserWait = 0;

	rz = 0;
	rzv = 0;
	state = 0;
	stateTimer = 0;
	exists = true;

	toggleMidLasers = false;
	toggleTopLasers = false;

	midLasersCharge = 0;
	topLasersCharge = 0;

	hp = 200; // set later

	b_hp = new int[]{12, 12, 12, 12};
	big_hp = 40;

	state = BOSS_BIGGUN_ENCOUNTERIDLE;
}

void Boss_BigGun::idle(float dt) {
	float vel = 3.0f;
	

	if (Engine::player != NULL){
		Player* player = Engine::player;
		GsVec ppos = Engine::getPlayerPosition();

		float wantZ = ppos.z - 7.0f;
		if (pos.z > wantZ){
			pos.z += (wantZ - pos.z)  * (1 - pow(0.1f, dt)) * (sin(t) + 2.0f);
		}
		/////// AI
		toggleMidLasers = false;
		toggleTopLasers = false;
		allowRotFix = true;

		float dx = ppos.x - pos.x;
		float dy = ppos.y - pos.y;

		if (state == BOSS_BIGGUN_ENCOUNTERIDLE){
			pos.x += dx * (1 - pow(0.85f, dt));
			pos.y += dy * (1 - pow(0.85f, dt));

			vel = 7.0f;

			if (pos.z > ppos.z - 15.0f){
				setNextState();
			}
		}

		if (state == BOSS_BIGGUN_IDLE){
			pos.x += dx * (1 - pow(0.9f, dt));
			pos.y += dy * (1 - pow(0.9f, dt));

			if (stateTimer > 4){
				setNextState();
			}
		}

		if (state == BOSS_BIGGUN_IDLE_AIM){
			toggleTopLasers = true;
			toggleMidLasers = true;
			pos.x += dx * (1 - pow(0.89f, dt));
			pos.y += dy * (1 - pow(0.89f, dt));

			GsVec dp = ppos - pos;

			dp.normalize();

			dir += (dp - dir) * (1 - pow(0.4f, dt));
			dir.normalize();

			if (stateTimer > 8){
				setNextState();
			}
		}

		if (state == BOSS_BIGGUN_LEFTRIGHT){
			//toggleMidLasers = true;
			GsVec dp = ppos - pos;

			dp.normalize();

			dir += (dp - dir) * (1 - pow(0.5f, dt));
			dir.normalize();


			if (stateTimer < 2){
				pos.x += RTools::easeInOutQuad((stateTimer), 0, -1, 2) * dt;
			}
			else if (stateTimer < 6){
				pos.x += RTools::easeInOutQuad((stateTimer - 2), 0, 2, 4)  * dt;
			}
			else{
				pos.x += RTools::easeInOutQuad((stateTimer - 6), 0, -1, 2) * dt;
			}

			if (stateTimer > 8){
				setNextState();
			}
		}

		if (state == BOSS_BIGGUN_BARRELROLL){
			toggleTopLasers = true;
			pos.x += dx * (1 - pow(0.85f, dt));
			pos.y += dy * (1 - pow(0.85f, dt));

			if (stateTimer < 8){
				allowRotFix = false;
				
				dir += (GsVec(0, 0, 1) - dir) * (1 - pow(0.4f, dt));
				dir.normalize();

				float speed = (stateTimer*stateTimer) / 4.0f - 0.05f;
				speed = min(speed, 2);

				rzv = speed;
			}else{
				if (stateTimer > 9){
					toggleTopLasers = false;
				}
				rzv *= pow(0.01f, dt);
				if (stateTimer > 10){
					rzv = 0;
					setNextState();
				}
			}
		}

		if (state == BOSS_BIGGUN_ROT90){
			pos.x += dx * (1 - pow(0.85f, dt));
			pos.y += dy * (1 - pow(0.85f, dt));

			if (stateTimer < 2.5f){
				allowRotFix = false;

				float speed = (stateTimer*stateTimer) / 2.0f - 0.05f;
				speed = min(speed, 1);

				rzv = speed;
			}else{
				rzv *= pow(0.01f, dt);
				if (stateTimer > 4){
					rzv = 0;
					setNextState();
				}
			}


		}

	}

	if (allowRotFix){
		// rotates to the closest of 90 degrees
		float closest = round(rz / AI_MATH_HALF_PI_F) * AI_MATH_HALF_PI_F;

		rzv += (closest - rz)  * (1 - pow(0.1f, dt));
		rzv *= pow(0.01f, dt);
	}

	if (toggleMidLasers){
		midLasersCharge += dt / 2.0f;
	}
	else{
		midLasersCharge = 0;
	}

	if (toggleTopLasers){
		topLasersCharge += dt / 2.0f;
	}
	else{
		topLasersCharge = 0;
	}

	//pos += dir * vel;
	/*
	float t = Engine::getTimer();
	dir.x = cos(t);
	dir.z = sin(t);
	*/

	// ~~~ Laser collision with player

	//
	float t = Engine::getTimer();

	GsMat scale, translate, rotX, rotY, rotZ, fixRot;
	scale.scale(BOSS_SCALE, BOSS_SCALE, BOSS_SCALE);
	translate.translation(pos.x, pos.y, pos.z);

	float rx = atan2(dir.x, dir.z);
	float ry = atan2(dir.y, sqrt(dir.z * dir.z + dir.x * dir.x));

	fixRot.roty(AI_MATH_HALF_PI_F);
	rotZ.rotz(rz);
	rotX.roty(rx); // + AI_MATH_PI_F
	rotY.rotx(-ry);

	GsMat transform = scale * rotZ * rotY * rotX * translate;
	//transform = transform.inverse();

	GsVec laserOriginsMid[] = {
		GsVec(-22, 9, 0), GsVec(22, 9, 0),
		GsVec(-22, 18, 0), GsVec(22, 18, 0),
		GsVec(-22, 28, 0), GsVec(22, 28, 0),
	};
	GsVec laserOriginsTop[] = {
		GsVec(-13.3f, 71, 0), GsVec(13.3f, 71, 0),
		GsVec(-13.3f, 81, 0), GsVec(13.3f, 81, 0),
		GsVec(-13.3f, 92, 0), GsVec(13.3f, 92, 0),
		GsVec(-13.3f, 103, 0), GsVec(13.3f, 103, 0),
		GsVec(-13.3f, 114, 0), GsVec(13.3f, 114, 0)

	};

	if (toggleMidLasers){
		for (int i = 0; i < 6; i++){
			GsVec lpos = laserOriginsMid[i];
			lpos = lpos * transform;

			if (midLasersCharge > 1){
				float str = sin((t + i)* 4.0f) * 0.8f + 1.4f;
				float dis = (midLasersCharge - 1);
				if (dis > .2f) dis = .2f;
				dis *= 5 * 80;

				Enemy::doLaser(lpos, dir, dis);
			}
		}
	}

	if (toggleTopLasers){
		for (int i = 0; i < 8; i++){
			GsVec lpos = laserOriginsTop[i];
			lpos = lpos * transform;

			if (topLasersCharge > 1){
				float str = sin((t + i)* 4.0f) * 0.8f + 1.4f;
				float dis = (topLasersCharge - 1);
				if (dis > .2f) dis = .2f;
				dis *= 5 * 80;

				Enemy::doLaser(lpos, dir, dis);
			}
		}
	}

	// ~~~ END laser collision with player


	pos += dir * (vel * dt);
	rz += rzv * dt;

	stateTimer += dt;
	t += dt;
}
void Boss_BigGun::draw(const GsMat& tr, const GsMat& pr) {
	GsMat scale, translate, rotX, rotY, rotZ, fixRot;
	scale.scale(BOSS_SCALE, BOSS_SCALE, BOSS_SCALE);
	translate.translation(pos.x, pos.y, pos.z);

	float rx = atan2(dir.x, dir.z);
	float ry = atan2(dir.y, sqrt(dir.z * dir.z + dir.x * dir.x));

	fixRot.roty(AI_MATH_HALF_PI_F);
	rotZ.rotz(rz);
	rotX.roty(rx); // + AI_MATH_PI_F
	rotY.rotx(-ry);

	GsMat transform = scale * rotZ * rotY * rotX * translate;
	ship->draw(fixRot * transform * tr, pr);


	///
	GsVec buttonLocations[] = {
		GsVec(40, 26, 49), GsVec(-40, 26, 49),
		GsVec(40, 1, 49), GsVec(-40, 1, 49)
	};

	GsVec bigButtonPos = GsVec(-1, 17, 33);

	bool bb = true;
	for (int i = 0; i < 4; i++){
		if (b_hp[i] > 0){
			bb = false;
			GsMat translateButton, scaleButton, buttonRot;
			buttonRot.rotx(AI_MATH_HALF_PI_F);
			scaleButton.scale(5, 5, 5);
			translateButton.translation((buttonLocations[i]).e);

			button->draw(buttonRot * scaleButton * translateButton * transform * tr, pr);
		}
	}

	if (bb && big_hp > 0){
		GsMat translateButton, scaleButton, buttonRot;
		buttonRot.rotx(AI_MATH_HALF_PI_F);
		scaleButton.scale(15, 15, 15);
		translateButton.translation((bigButtonPos).e);

		button->draw(buttonRot * scaleButton * translateButton * transform * tr, pr);
	}


}
void Boss_BigGun::addDraw(const GsMat& tr, const GsMat& pr) {
	float t = Engine::getTimer();

	GsMat scale, translate, rotX, rotY, rotZ, fixRot;
	scale.scale(BOSS_SCALE, BOSS_SCALE, BOSS_SCALE);
	translate.translation(pos.x, pos.y, pos.z);

	float rx = atan2(dir.x, dir.z);
	float ry = atan2(dir.y, sqrt(dir.z * dir.z + dir.x * dir.x));

	fixRot.roty(AI_MATH_HALF_PI_F);
	rotZ.rotz(rz);
	rotX.roty(rx); // + AI_MATH_PI_F
	rotY.rotx(-ry);

	GsMat transform = scale * rotZ * rotY * rotX * translate;
	//transform = transform.inverse();

	GsVec laserOriginsMid[] = {
		GsVec(-22, 9, 0), GsVec(22, 9, 0),
		GsVec(-22, 18, 0), GsVec(22, 18, 0),
		GsVec(-22, 28, 0), GsVec(22, 28, 0),
	};
	GsVec laserOriginsTop[] = {
		GsVec(-13.3f, 71, 0), GsVec(13.3f, 71, 0),
		GsVec(-13.3f, 81, 0), GsVec(13.3f, 81, 0),
		GsVec(-13.3f, 92, 0), GsVec(13.3f, 92, 0),
		GsVec(-13.3f, 103, 0), GsVec(13.3f, 103, 0),
		GsVec(-13.3f, 114, 0), GsVec(13.3f, 114, 0)
	
	};

	if (toggleMidLasers){
		for (int i = 0; i < 6; i++){
			GsVec lpos = laserOriginsMid[i];
			lpos = lpos * transform;

			if (midLasersCharge > 1){
				float str = sin((t + i)* 4.0f) * 0.8f + 1.4f;
				float dis = (midLasersCharge - 1);
				if (dis > .2f) dis = .2f;
				dis *= 5 * 80;

				Enemy::doLaserDraw(lpos, dir, dis, str, tr, pr);
			}
			else{
				float str = midLasersCharge + .2f;
				Enemy::doLaserHintDraw(lpos, dir, 80.0f, str, tr, pr);
			}
		}
	}
	
	if (toggleTopLasers){
		for (int i = 0; i < 8; i++){
			GsVec lpos = laserOriginsTop[i];
			lpos = lpos * transform;

			if (topLasersCharge > 1){
				float str = sin((t + i)* 4.0f) * 0.8f + 1.4f;
				float dis = (topLasersCharge - 1);
				if (dis > .2f) dis = .2f;
				dis *= 5 * 80;

				Enemy::doLaserDraw(lpos, dir, dis, str, tr, pr);
			}
			else{
				float str = topLasersCharge + .2f;
				Enemy::doLaserHintDraw(lpos, dir, 80.0f, str, tr, pr);
			}
		}
	}
	
	shipLights->draw(fixRot * transform * tr, pr);

}

void Boss_BigGun::postDraw(const GsMat& tr, const GsMat& pr) {
	float t = Engine::getTimer();

	GsMat scale, translate, rotX, rotY, rotZ, fixRot;
	scale.scale(BOSS_SCALE, BOSS_SCALE, BOSS_SCALE);
	translate.translation(pos.x, pos.y, pos.z);

	float rx = atan2(dir.x, dir.z);
	float ry = atan2(dir.y, sqrt(dir.z * dir.z + dir.x * dir.x));

	fixRot.roty(AI_MATH_HALF_PI_F);
	rotZ.rotz(rz);
	rotX.roty(rx); // + AI_MATH_PI_F
	rotY.rotx(-ry);

	GsMat transform = scale * rotZ * rotY * rotX * translate;
	//transform = transform.inverse();

	shipLights->draw(fixRot * transform * tr, pr);

}

void Boss_BigGun::putLights(){
	DeferredUtils::addLight(Light(pos - dir * .2f, 5, RColor(0.9f, .1f, 0.1f, 1)));

	//
	float t = Engine::getTimer();

	float str = sin(t* 4.0f) * 0.8f + 1.4f;

	if (toggleMidLasers){
		if (midLasersCharge > 1){
			DeferredUtils::addLight(Light(pos + dir * 7.0f, 15 + str * 5, RColor(0.9f, .1f, 0.1f, str)));
		}
		
	}

	if (toggleTopLasers){
		if (topLasersCharge > 1){
			DeferredUtils::addLight(Light(pos + dir * 7.0f, 30 + str * 5, RColor(0.9f, .1f, 0.1f, str)));
		}
	}
}


bool Boss_BigGun::projectileCollisionCheck(GsVec center, float radius, DamageData& data){
	bool hit = false;

	GsMat scale, translate, rotX, rotY, rotZ, fixRot;
	scale.scale(BOSS_SCALE, BOSS_SCALE, BOSS_SCALE);
	translate.translation(pos.x, pos.y, pos.z);

	float rx = atan2(dir.x, dir.z);
	float ry = atan2(dir.y, sqrt(dir.z * dir.z + dir.x * dir.x));

	fixRot.roty(AI_MATH_HALF_PI_F);
	rotZ.rotz(rz);
	rotX.roty(rx); // + AI_MATH_PI_F
	rotY.rotx(-ry);

	GsMat transform = scale * rotZ * rotY * rotX * translate;
	///
	GsVec buttonLocations[] = {
		GsVec(40, 26, 49), GsVec(-40, 26, 49),
		GsVec(40, 1, 49), GsVec(-40, 1, 49)
	};

	GsVec bigButtonPos = GsVec(-1, 17, 33);
	bool bb = true;
	for (int i = 0; i < 4; i++){
		if (b_hp[i] > 0){
			bb = false;
			
			GsVec bpos = buttonLocations[i] * transform;
			if ((bpos - center).len() < 0.5f){
				b_hp[i]--;
				data.onhit(data, this);

				addSmallExplosion(bpos);
				hit = true;
				break;
			}
		}
	}

	if (bb && big_hp > 0){
		GsVec bpos = bigButtonPos * transform;
		if ((bpos - center).len() < 0.5f){
			big_hp--;
			data.onhit(data, this);

			addSmallExplosion(bpos);

			hit = true;
		}
	}

	hp = big_hp + b_hp[0] + b_hp[1] + b_hp[2] + b_hp[3];

	if (hp <= 0){

		for (int i = 0; i < 100; i++){
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
			rndvel.z -= 2.05f;

			float rndsize = (float)(gs_randomd()*gs_randomd() * 6.2f) + 0.1f;

			float rndlife = (float)(gs_randomd() * gs_randomd() * 5.0f) + 0.8f;

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

		// Draw a bunch of particles:
		GLTexture* tex = Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp");

		std::vector<Particle*>* list = ParticleUtils::getParticleList(Engine::particles, tex->texId);

		for (int i = 0; i < 10000; i++){
			GsVec rndvel = GsVec((float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1), (float)(gs_randomd() * 2 - 1));
			rndvel.normalize();

			rndvel *= (float)(gs_randomd()*gs_randomd() * 2.0f);
			rndvel.z -= 2.05f;

			float rndsize = (float)(gs_randomd()*gs_randomd() * .45f) + .01f;

			float rndlife = (float)(gs_randomd() * gs_randomd() * 5.4f) + .1f;

			Particle_SimpleDecay* particle = (ParticleBank::simpleDecay[ParticleBank::simpleDecay_num++]);

			if (ParticleBank::simpleDecay_num >= PARTICLEBANK_NUM_SIMPLEDECAY){
				ParticleBank::simpleDecay_num = 0;
			}
			particle->exists = true;
			particle->del = false;
			particle->pos = pos;
			particle->initialSize = rndsize;
			particle->color = RColor(1, .2f, .5f, (float)gs_randomd());
			particle->vel = rndvel;
			particle->duration = rndlife * 5;
			particle->t = 0;

			list->push_back(particle);
		}

		exists = false;
	}
	return hit;
}

void Boss_BigGun::addSmallExplosion(GsVec c){
	for (int i = 0; i < 4; i++){
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

		float rndsize = (float)(gs_randomd() * 0.2f) + 0.1f;

		float rndlife = (float)(gs_randomd() * gs_randomd() * 1.0f) + 0.8f;

		Particle_ExplosionDecay* particle = (ParticleBank::explosionDecay[ParticleBank::explosionDecay_num++]);

		if (ParticleBank::explosionDecay_num >= PARTICLEBANK_NUM_EXPLOSIONDECAY){
			ParticleBank::explosionDecay_num = 0;
		}
		particle->exists = true;
		particle->del = false;
		particle->pos = c;
		particle->initialSize = rndsize;
		particle->color = RColor(1, 1, 1, (float)gs_randomd());
		particle->vel = rndvel;
		particle->duration = rndlife;
		particle->t = 0;
		particle->rotSpeed = (float)(gs_randomd() * 2 - 1) * 4.0f;

		list->push_back(particle);
	}
}