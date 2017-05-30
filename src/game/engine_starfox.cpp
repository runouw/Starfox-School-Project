#include "engine_starfox.h"

#include <game\enemy\enemies.h>

using namespace GLTools;

Engine_Starfox::Engine_Starfox(){
	
}
void Engine_Starfox::init(int w, int h){
	GLActivity::init(w, h);

	t = 0;

	oldmx = 0;
	oldmy = 0;

	camRotX = 0;
	camRotY = 0;
	cx = 0;
	cy = 0;
	cz = -3;

	// default material:
	Material* material = new Material();
	material->program = Shaders::getSimpleShader();
	material->tex = Textures::get("../res/models/arwing/arwing_01.bmp");
	Mesh::setDefaultMaterial(material);

	// defualt program:
	Material::setDefaultProgram(Shaders::getSimpleShader());

	player = new Player();
	player->init();

	// now load scene:
	scene = new Scene_Asteroid1();
	scene->init();

	//quad = new GLO_Quad(Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp"));
	//quad->init(program);

	//geoFormats = new GLuint[]{GL_RGBA8, GL_RGBA8, GL_RGB16F, GL_RGB16F};
	geoFormats[0] = GL_RGBA8;
	geoFormats[1] = GL_RGBA8;
	geoFormats[2] = GL_RGB16F;
	geoFormats[3] = GL_RGB16F;

	geo = new GLGeometrybuffer(width, height, 4, geoFormats, true);

	quad = new GLO_Quad(geo->getTexture(0));
	quad->init(*Shaders::getSimpleShader());

	enemies.clear();
	projectiles.clear();
	particles = Particles(); // this clears

	Enemy_Ship1::init();
	Enemy_Ship2::init();
	Boss_BigGun::init();

	// Pre load some textures for particle animations:::
	
	ParticleUtils::getParticleList(Engine::particles, Textures::get("../res/images/bakuhatu01.bmp")->texId);
	ParticleUtils::getParticleList(Engine::particles, Textures::get("../res/images/bakuhatu02.bmp")->texId);
	ParticleUtils::getParticleList(Engine::particles, Textures::get("../res/images/bakuhatu03.bmp")->texId);
	ParticleUtils::getParticleList(Engine::particles, Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp")->texId);
}
void Engine_Starfox::changedWH(){
	printf("Resize called: (%d, %d)\n", width, height);
	glViewport(0, 0, width, height);
	
	geo->clear();
	delete geo;
	geo = new GLGeometrybuffer(width, height, 4, geoFormats, true);
}

void Engine_Starfox::mouseDown(int button){
	oldmx = mx;
	oldmy = my;
}
void Engine_Starfox::mouseUp(int button){

}

// Do keyboard, mouse and timestep
void Engine_Starfox::idle(float dt){
	t += dt;

	player->idle(dt);

	cx += (-player->x * .8f - cx) * (1 - pow(0.01f, dt));
	cy += (-player->y * .8f - cy) * (1 - pow(0.01f, dt));

	RTools::elasticBounds(cx, 2.0f, .003f, dt);
	RTools::elasticBounds(cy, 1.5f, .003f, dt);

	for (size_t i = 0; i < projectiles.size(); i++){
		projectiles[i]->idle(dt);
		if (projectiles[i]->exists == false){
			delete projectiles[i];
			projectiles[i] = projectiles[projectiles.size() - 1];
			projectiles.pop_back();
			i--;
		}
	}

	for (size_t i = 0; i < enemies.size(); i++){
		enemies[i]->idle(dt);
		if (enemies[i]->exists == false){
			delete enemies[i];
			enemies[i] = enemies[enemies.size() - 1];
			enemies.pop_back();
			i--;
		}
	}

	ParticleUtils::idle(particles, dt);

	if (Keys::justPressed(' ')){
		GsVec ppos = GsVec(player->x, player->y, 0);
		ppos = ppos * (scene->resultMat).inverse();

		GsVec dir = GsVec(player->px - player->x, player->py - player->y, -RETICLE_DISTANCE);
		dir.normalize();

		ppos += dir * .4f;

		Laser* laser = new Laser(ppos, dir);

		projectiles.push_back(laser);
	}


	// TODO: advance by arwing speed

	scene->advance(dt * 1);
}

// Do render only
void Engine_Starfox::render(){
	GLActivity::render();

	GsMat proj;
	//proj.ortho(-1, 1, -1, 1, 1, -1);
	//proj.perspective(45, (float)width / (float)height, .01f, 2000);

	// No more far clipping plane!! http://chaosinmotion.com/blog/?p=555
	float fov = 1.0f / tan((45 / (2.0f * 3.14159f)) / 2.0f);
	float aspect = (float)width / (float)height;

	float mat[] = {
		fov / aspect, 0, 0, 0,
		0, fov, 0, 0,
		0, 0, 0, -1,
		0, 0, -.1f, 0
	};
	proj.set(mat);

	GsMat cpos;
	cpos.translation(cx, cy, cz);

	GsMat rotx, roty;
	rotx.rotx(camRotY);
	roty.roty(camRotX);


	cam = roty * rotx;
	GsMat camNoTranslate = cam;
	camAntiRot = cam.inverse();
	cam = cpos * cam;

	cam = cam * scene->resultMat;
	GsMat resultMatInv = scene->resultMat;
	resultMatInv.invert();

	geo->bindFramebuffer();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->draw(GsMat(), cam *proj);

	player->draw(resultMatInv, cam * proj);

	for (size_t i = 0; i < projectiles.size(); i++){
		projectiles[i]->draw(GsMat(), cam * proj);
	}
	for (size_t i = 0; i < enemies.size(); i++){
		enemies[i]->draw(GsMat(), cam * proj);
	}
	
	geo->unbindFramebuffer();
	glViewport(0, 0, width, height);
	glClearColor(.5f, .5f, cos(t)*0.5f + 0.5f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	scene->drawSky(GsMat(), camNoTranslate * proj);

	// ~~~ Deferred Lighting ~~~
	// Draw in ambient:
	glDisable(GL_DEPTH_TEST);

	// Lights... TOOD: move

	GsVec ppos = GsVec(player->x, player->y, 0);
	ppos = ppos * resultMatInv;

	DeferredUtils::addLight(Light(ppos, 15, RColor(1, 1, 1, .6f)));
	for (size_t i = 0; i < projectiles.size(); i++){
		projectiles[i]->putLights();
	}
	for (size_t i = 0; i < enemies.size(); i++){
		enemies[i]->putLights();
	}

	// ^^^

	DeferredUtils::setGlobalLight(GlobalLight(GsVec(1, 1, 1), RColor(.5f, .5f, .5f, 1), RColor(.5f, .5f, .5f, 1)));
	DeferredUtils::applyGlobalLighting(*(geo->getTexture(0)), *(geo->getTexture(1)), *(geo->getTexture(2)));

	DeferredUtils::applyLighting(*(geo->getTexture(1)), *(geo->getTexture(2)), *(geo->getDepthTexture()), cam * proj);

	glEnable(GL_DEPTH_TEST);

	// COPY DEPTH BUFFER TO BACK BUFFER
	DeferredUtils::drawTextureToDepth(*(geo->getDepthTexture()));

	// ~~~ End deferred lighting ~~~	

	// ~~~ POST DRAW
	scene->postDraw(GsMat(), cam *proj);

	for (size_t i = 0; i < projectiles.size(); i++){
		projectiles[i]->postDraw(GsMat(), cam * proj);
	}
	for (size_t i = 0; i < enemies.size(); i++){
		enemies[i]->postDraw(GsMat(), cam * proj);
	}
	// ~~~ ADD DRAW

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	// Additive drawing done here
	scene->addDraw(GsMat(), cam *proj);
	player->addDraw(resultMatInv, cam * proj);
	for (size_t i = 0; i < projectiles.size(); i++){
		projectiles[i]->addDraw(GsMat(), cam * proj);
	}
	for (size_t i = 0; i < enemies.size(); i++){
		enemies[i]->addDraw(GsMat(), cam * proj);
	}

	ParticleUtils::draw(particles, cam, proj);

	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	player->drawCursor(resultMatInv, cam * proj);

	

	if (Keys::down('1')){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Draw framebuffers!
		GsMat corners;

		// top left
		corners.translation(-.5f, .5f, 0);
		quad->setTexture(geo->getTexture(0));
		quad->draw(corners, GsMat());

		// top right
		corners.translation(.5f, .5f, 0);
		quad->setTexture(geo->getTexture(1));
		quad->draw(corners, GsMat());

		// bottom left
		corners.translation(-.5f, -.5f, 0);
		quad->setTexture(geo->getTexture(2));
		quad->draw(corners, GsMat());

		// bottom right
		corners.translation(.5f, -.5f, 0);
		quad->setTexture(geo->getDepthTexture());
		quad->draw(corners, GsMat());
	}

}