#include "player.h"

#include <rtools\keys.h>
#include <game\engine.h>

Player::Player(){
	arwing = new Arwing();

	x = 0;
	y = 0;

	vx = 0;
	vy = 0;

	cvx = 0;
	cvy = 0;

	freeHW = 4;
	freeHH = 3;

	tilt = 0;
	tiltV = 0;

	cursorQuad = new GLO_Quad(Textures::get("../res/images/reticle.bmp"));
}
void Player::init(){
	arwing->init();
	cursorQuad->init(*Shaders::getReticleShader());
}

void Player::idle(float dt){
	// Movements
	//float t = Engine::getTimer();
	float t = 0;

	float speed = .2f * dt;
	if (Keys::down('a') != Keys::down('d')){
		if (Keys::down('a')){
			vx += (-2.5f * RETICLE_DISTANCE - vx) * (1 - pow(0.05f, dt));
		}else{
			vx += (2.5f * RETICLE_DISTANCE - vx) * (1 - pow(0.05f, dt));
		}
	}else{
		vx *= pow(0.001f, dt);
	}
	
	if (Keys::down('w') != Keys::down('s')){
		if (Keys::down('w')){
			vy += (2.5f * RETICLE_DISTANCE - vy) * (1 - pow(0.05f, dt));
		}else{
			vy += (-2.5f * RETICLE_DISTANCE - vy) * (1 - pow(0.05f, dt));
		}
	}else{
		vy *= pow(0.001f, dt);
	}
	
	if (Keys::justPressed('q')){
		if (tiltV < -1.5f){
			tiltV += -2;
			tiltV *= 3.3f;
		}
		tiltV += -6;
		tiltV = max(tiltV, -60);
	}
	if (Keys::justPressed('e')){
		if (tiltV > 1.5f){
			tiltV += 2;
			tiltV *= 3.3f;
		}
		tiltV += 6;
		tiltV = min(tiltV, 60);
	}

	if (Keys::down('q') != Keys::down('e')){
		if (Keys::down('q')){
			tiltV += (-AI_MATH_HALF_PI_F - tilt) * (1 - pow(0.05f, dt)) * 5.0f;
			tilt += (-AI_MATH_HALF_PI_F - tilt) * (1 - pow(0.05f, dt));
		}
		else{
			tiltV += (AI_MATH_HALF_PI_F - tilt) * (1 - pow(0.05f, dt)) * 5.0f;
			tilt += (AI_MATH_HALF_PI_F - tilt) * (1 - pow(0.05f, dt));
		}
	}else{
		tiltV += (0 - tilt) * (1 - pow(0.1f, dt)) * 2.0f;
		tilt += (0 - tilt) * (1 - pow(0.05f, dt));
	}

	tiltV *= pow(0.02f, dt);

	tilt += tiltV * dt;
	if (tilt > AI_MATH_PI_F){
		tilt -= AI_MATH_TWO_PI_F;
	}
	if (tilt < -AI_MATH_PI_F){
		tilt += AI_MATH_TWO_PI_F;
	}

	px += vx * dt;
	py += vy * dt;

	RTools::elasticBounds(px, freeHW, .01f, dt);
	RTools::elasticBounds(py, freeHH, .01f, dt);

	float dx = px - x;
	float dy = py - y;

	cvx += (px * .94f - x) * (1 - pow(0.001f, dt));
	cvy += (py * .94f - y) * (1 - pow(0.001f, dt));

	//cvx = min(max(-7.0f, cvx), 7.0f);
	//cvy = min(max(-7.0f, cvy), 7.0f);

	cvx *= pow(0.001f, dt);
	cvy *= pow(0.001f, dt);

	x += cvx * dt;
	y += cvy * dt;

	RTools::elasticBounds(x, freeHW, .01f, dt);
	RTools::elasticBounds(y, freeHH, .01f, dt);

	// drift::
	x += sin(t * 0.8f) * 0.12f * dt;
	y += cos(t * 0.63f) * 0.08f * dt;
	//tilt += cos(t * 0.8f) * 0.2f * dt;

	GsVec cursor3d = GsVec(px, py, -RETICLE_DISTANCE);
	GsVec arwing3d = GsVec(x, y, 0);
	GsVec d3d = arwing3d - cursor3d;
	float dxn = atan2(d3d.x, d3d.z);
	float dyn = atan2(d3d.y, d3d.z);


	//float rotX = dyn * 1.2f;
	//float rotY = AI_MATH_PI_F + dxn * -.8f;
	//float rotZ = dxn * -.8f;

	float rotX = dyn;
	float rotY = dxn;
	float rotZ = -(dxn / (abs(tilt) + 1)) * 1.2f + tilt + cos(t * 0.8f) * 0.2f * dt;


	arwing->wingTilt = (dyn / (1 + abs(dxn))) * -0.8f;

	GsMat rotFlip, tr_pos, tr_rotX, tr_rotY, tr_rotZ;
	rotFlip.roty(AI_MATH_PI_F);
	tr_pos.translation(x, y, 0);
	tr_rotX.rotx(rotX);
	tr_rotY.roty(rotY);
	tr_rotZ.rotz(rotZ);
	transform = tr_rotZ * tr_rotX * tr_rotY * rotFlip * tr_pos;


	{
		GsVec ppos = GsVec(x, y, 0);
		ppos = ppos * (Engine::scene->resultMat).inverse();

		GsVec dir = GsVec(px - x, py - y, -RETICLE_DISTANCE);
		dir.normalize();

		ppos -= dir * .4f;
		///
		for (int i = 0; i < 3; i++){
			GsVec rndpos = ppos + GsVec((float)(gs_randomd() * 2 - 1) * .2f, 0, (float)(gs_randomd() * 2 - 1)) * .1f;
			GsVec rndvel = GsVec((float)(gs_randomd() * 2 - 1) * .2f, (float)(gs_randomd() * 2 - 1) * .2f, (float)(gs_randomd() * 2 - 1) + 2);
			rndvel *= (float)(gs_randomd()*gs_randomd() * 0.2f) + .05f;

			rndvel = rndvel * (tr_rotZ * tr_rotX * tr_rotY).inverse();

			float rotSpeed = (float)(gs_randomd() * .4f) - .2f;

			float rndSize = (float)(gs_randomd() * .6f) - .1f;
			float rndLife = (float)(gs_randomd() * 0.6f) - .1f;

			ParticleUtils::addParticle(Engine::particles, new Particle_SimpleDecay(Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp"), rndpos, rndSize, RColor(.3f, .3f, .3f, 1), rndvel, rndLife));
		}
	}

	if (abs(tiltV) > 10.5f){
		{
			
			///
			for (int i = 0; i < 3; i++){
				GsVec ppos = GsVec(x, y, 0);
				ppos = ppos * (Engine::scene->resultMat).inverse();

				GsVec dir = GsVec(px - x, py - y, -RETICLE_DISTANCE);
				dir.normalize();

				ppos += GsVec(sin(tilt + i*tiltV / 60.0f + AI_MATH_HALF_PI_F), cos(tilt + i*tiltV / 60.0f + AI_MATH_HALF_PI_F), 0) * .45f;

				GsVec rndpos = ppos + GsVec((float)(gs_randomd() * 2 - 1) * .2f, 0, (float)(gs_randomd() * 2 - 1)) * .1f;
				GsVec rndvel = GsVec((float)(gs_randomd() * 2 - 1) * .2f, (float)(gs_randomd() * 2 - 1) * .2f, (float)(gs_randomd() * 2 - 1) + 2);
				rndvel *= (float)(gs_randomd()*gs_randomd() * 0.2f) + .05f;

				rndvel = rndvel * (tr_rotZ * tr_rotX * tr_rotY).inverse();

				float rotSpeed = (float)(gs_randomd() * .4f) - .2f;

				float rndSize = (float)(gs_randomd() * .6f) + .2f;
				float rndLife = (float)(gs_randomd() * 0.6f) - .1f;

				ParticleUtils::addParticle(Engine::particles, new Particle_SimpleDecay(Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp"), rndpos, rndSize, RColor(.1f, .1f, .3f, 1), rndvel, rndLife));
			}

			for (int i = 0; i < 3; i++){
				GsVec ppos = GsVec(x, y, 0);
				ppos = ppos * (Engine::scene->resultMat).inverse();

				GsVec dir = GsVec(px - x, py - y, -RETICLE_DISTANCE);
				dir.normalize();

				ppos += GsVec(sin(tilt + i*tiltV / 60.0f - AI_MATH_HALF_PI_F), cos(tilt + i*tiltV / 60.0f - AI_MATH_HALF_PI_F), 0) * .45f;

				GsVec rndpos = ppos + GsVec((float)(gs_randomd() * 2 - 1) * .2f, 0, (float)(gs_randomd() * 2 - 1)) * .1f;
				GsVec rndvel = GsVec((float)(gs_randomd() * 2 - 1) * .2f, (float)(gs_randomd() * 2 - 1) * .2f, (float)(gs_randomd() * 2 - 1) + 2);
				rndvel *= (float)(gs_randomd()*gs_randomd() * 0.2f) + .05f;

				rndvel = rndvel * (tr_rotZ * tr_rotX * tr_rotY).inverse();

				float rotSpeed = (float)(gs_randomd() * .4f) - .2f;

				float rndSize = (float)(gs_randomd() * .6f) + .2f;
				float rndLife = (float)(gs_randomd() * 0.6f) - .1f;

				ParticleUtils::addParticle(Engine::particles, new Particle_SimpleDecay(Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp"), rndpos, rndSize, RColor(.1f, .1f, .3f, 1), rndvel, rndLife));
			}
		}
	}



}
void Player::draw(const GsMat& tr, const GsMat& pr){
	arwing->draw(transform * tr, pr);
}
void Player::addDraw(const GsMat& tr, const GsMat& pr){
	arwing->addDraw(transform * tr, pr);
}

void Player::drawCursor(const GsMat& tr, const GsMat& pr){
	glDisable(GL_DEPTH_TEST);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GsMat pmat, pscale, pmat2, pscale2;
	pmat.translation(px, py, -RETICLE_DISTANCE);

	float dx = px - x;
	float dy = py - y;

	pmat2.translation(x + dx * 2.0f, y + dy * 2.0f, -RETICLE_DISTANCE * 2.0f);
	pscale.scale(1.0f, 1.0f, 1.0f);
	pscale2.scale(1.0f, 1.0f, 1.0f);

	cursorQuad->draw(pscale2 * pmat2 * Engine::getCamAntiRot() * RTools::billboardify(tr), pr);

	cursorQuad->draw(pscale * pmat * Engine::getCamAntiRot() * RTools::billboardify(tr), pr);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

float Player::lineIntersect(const GsVec& start, const GsVec& end){
	// Confusing! Future work on this must be better
	GsMat trInv = Engine::scene->resultMat * transform.inverse();
	GsVec _start = start * trInv;
	GsVec _end = end * trInv;

	return arwing->lineIntersect(_start, _end);
}