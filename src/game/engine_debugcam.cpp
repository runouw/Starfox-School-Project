#include "engine_debugcam.h"

using namespace GLTools;

Model* ship1;
Model* ship2;
Model* redShip1;
Model* redBigGunShip;
Model* redBigGunShip_lights;

Engine_DebugCam::Engine_DebugCam(){
}
void Engine_DebugCam::init(int w, int h){
	GLActivity::init(w, h);

	t = 0;
	dragCam = false;
	oldmx = 0;
	oldmy = 0;
	camRotX = 0;
	camRotY = 0;
	cx = 0;
	cy = 0;
	cz = -1;

	// default material:
	Material* material = new Material();
	material->program = Shaders::getSimpleShader();
	material->tex = Textures::get("../res/models/arwing/arwing_01.bmp");
	Mesh::setDefaultMaterial(material);

	// defualt program:
	Material::setDefaultProgram(Shaders::getDeferredShader());

	// Load arwing
	arwing = new Arwing();
	arwing->init();

	//player = new Player();
	//player->init();

	// load scene
	scene = new Scene_Asteroid1();
	scene->init();

	Material::setDefaultProgram(Shaders::getSimpleShader());

	//quad = new GLO_Quad(Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp"));
	//quad->init(program);

	// TODO: remove position buffer because it can be derived...
	// Color (RBGA), Bloom (RGB), position (RGBA), normals (RGB)
	
	geoFormats = new GLuint[]{GL_RGBA8, GL_RGBA8, GL_RGB16F, GL_RGB16F};
	geo = new GLGeometrybuffer(width, height, 4, geoFormats, true);

	quad = new GLO_Quad(geo->getTexture(0));
	quad->init(*Shaders::getSimpleShader());

	showBuffers = false;

	{
		Material::setDefaultProgram(Shaders::getDeferredShader());

		// to load textures from correct location:
		Material::setBasePath("../res/models/armada/");
		const struct aiScene* scene = aiImportFile("../res/models/armada/ship1.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!");
		}

		// extract model from OBJ:
		ship1 = new Model(scene, scene->mRootNode);
		ship1->init();

	}
	{
		Material::setDefaultProgram(Shaders::getDeferredShader());

		// to load textures from correct location:
		Material::setBasePath("../res/models/armada/");
		const struct aiScene* scene = aiImportFile("../res/models/armada/ship2.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!");
		}

		// extract model from OBJ:
		ship2 = new Model(scene, scene->mRootNode);
		ship2->init();

	}
	
	{
		Material::setDefaultProgram(Shaders::getDeferredShader());

		// to load textures from correct location:
		Material::setBasePath("../res/models/armada/");
		const struct aiScene* scene = aiImportFile("../res/models/armada/redship1.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!");
		}

		// extract model from OBJ:
		redShip1 = new Model(scene, scene->mRootNode);
		redShip1->init();
	}
	
	{
		Material::setDefaultProgram(Shaders::getDeferredShader());

		// to load textures from correct location:
		Material::setBasePath("../res/models/armada/");
		const struct aiScene* scene = aiImportFile("../res/models/armada/redgunship1.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!\n");
		}

		// extract model from OBJ:
		redBigGunShip = new Model(scene, scene->mRootNode);
		redBigGunShip->init();
	}
	{
		Material::setDefaultProgram(Shaders::getSimpleShader());

		// to load textures from correct location:
		Material::setBasePath("../res/models/armada/");
		const struct aiScene* scene = aiImportFile("../res/models/armada/redgunship1_lights.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!\n");
		}

		// extract model from OBJ:
		redBigGunShip_lights = new Model(scene, scene->mRootNode);
		redBigGunShip_lights->init();
	}

	particles = Particles();

	enemies.clear();
	projectiles.clear();
}

void Engine_DebugCam::changedWH(){
	printf("Resize called: (%d, %d)\n", width, height);
	glViewport(0, 0, width, height);

	geo->clear();
	delete geo;
	geo = new GLGeometrybuffer(width, height, 4, geoFormats, true);
}

void Engine_DebugCam::mouseDown(int button){
	dragCam = true;
	oldmx = mx;
	oldmy = my;
}
void Engine_DebugCam::mouseUp(int button){
	dragCam = false;
}

// Do keyboard, mouse and timestep
void Engine_DebugCam::idle(float dt){
	t += dt;

	if (dragCam){
		float dmx = (float)(mx - oldmx) / (float)width;
		float dmy = (float)(my - oldmy) / (float)height;

		camRotX += AI_MATH_TWO_PI_F * dmx;
		camRotY += AI_MATH_TWO_PI_F * dmy;
	}

	oldmx = mx;
	oldmy = my;

	float speed = 20.0f * dt;
	if (Keys::down(GLUT_KEY_SHIFT_L) || Keys::down(GLUT_KEY_SHIFT_R)){
		speed *= 4.0f;
	}

	if (Keys::down('w')){
		vz += cos(camRotX) * speed;
		vx -= sin(camRotX) * speed;
		vy += sin(camRotY) * speed;
	}
	if (Keys::down('s')){
		vz -= cos(camRotX) * speed;
		vx += sin(camRotX) * speed;
		vy -= sin(camRotY) * speed;
	}
	if (Keys::down('d')){
		vz += cos(camRotX + AI_MATH_HALF_PI_F) * speed;
		vx -= sin(camRotX + AI_MATH_HALF_PI_F) * speed;
	}
	if (Keys::down('a')){
		vz += cos(camRotX - AI_MATH_HALF_PI_F) * speed;
		vx -= sin(camRotX - AI_MATH_HALF_PI_F) * speed;
	}

	if (Keys::justPressed('1')){
		showBuffers = !showBuffers;
	}

	cx += vx * dt;
	cy += vy * dt;
	cz += vz * dt;

	vx *= pow(0.01f, dt);
	vy *= pow(0.01f, dt);
	vz *= pow(0.01f, dt);

	ParticleUtils::idle(particles, dt);


	GsMat tr, arwingTr, arwingRot, arwingRotY;

	arwingRotY.roty(AI_MATH_PI_F);
	arwingTr.translation(sin(t*0.7f)*.1f, sin(t*1.2f)*.07f, 0/*-t / 2.0f*/);
	arwingRot.rotz(sin(t*1.4f) * -.1f);

	GsVec ppos = GsVec(0, 0, -.4f) * arwingRotY * arwingTr * arwingRot;

	for (int i = 0; i < 3; i++){
		GsVec rndpos = ppos + GsVec((float)(gs_randomd() * 2 - 1) * .2f, 0, (float)(gs_randomd() * 2 - 1)) * .1f;
		GsVec rndvel = GsVec((float)(gs_randomd() * 2 - 1) * .2f, (float)(gs_randomd() * 2 - 1) * .2f, (float)(gs_randomd() * 2 - 1) + 2);
		rndvel *= (float)(gs_randomd()*gs_randomd() * 1.0f) + .2f;


		float rotSpeed = (float)(gs_randomd() * 4) - 2;

		float rndSize = (float)(gs_randomd() * .4f) - .05f;
		float rndLife = (float)(gs_randomd() * 4.4f) - .4f;

		ParticleUtils::addParticle(particles, new Particle_ExplosionDecay(Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp"), rndpos, rndSize, RColor(1, 1, 1, 1), rndvel, rndLife, rotSpeed));
	}
}

// Do render only
void Engine_DebugCam::render(){
	GLActivity::render();

	GsMat proj;
	//proj.ortho(-1, 1, -1, 1, 1, -1);
	//proj.perspective(45, (float)width / (float)height, .01f, 100);

	// No more far clipping plane!! http://chaosinmotion.com/blog/?p=555
	float fov = 1.0f/tan((45 / (2.0f * 3.14159f))/2.0f);
	float aspect = (float)width / (float)height;

	float mat[] = {
		fov / aspect, 0, 0, 0,
		0, fov, 0, 0,
		0, 0, 0, -1,
		0, 0, -.01f, 0
	};
	proj.set(mat);

	GsMat cpos;
	cpos.translation(cx, cy, cz);

	GsMat rotx, roty;
	rotx.rotx(camRotY);
	roty.roty(camRotX);

	GsMat camNoTranslate;

	cam = roty * rotx;
	camNoTranslate = cam;
	camAntiRot = cam.inverse();
	cam = cpos * cam;

	//
	GsMat tr, arwingTr, arwingRot, arwingRotY;

	arwingRotY.roty(AI_MATH_PI_F);
	arwingTr.translation(sin(t*0.7f)*.1f, sin(t*1.2f)*.07f, 0/*-t / 2.0f*/);
	arwingRot.rotz(sin(t*1.4f) * -.1f);
	//

	geo->bindFramebuffer();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	arwing->draw(arwingRotY * arwingTr * arwingRot, cam * proj);

	GsMat ship1_scale, ship1_translate;
	ship1_scale.scale(.01f, .01f, .01f);
	ship1_translate.translation(1 + sin(t*0.8f + .5f)*.1f, sin(t*1.12f + .5f)*.07f, 0);
	ship1->draw(ship1_scale * ship1_translate, cam*proj);

	GsMat ship2_scale, ship2_translate;
	ship2_scale.scale(.01f, .01f, .01f);
	ship2_translate.translation(2 + sin(t*0.75f + .2f)*.1f, sin(t*1.23f + .2f)*.03f, 0);
	ship2->draw(ship2_scale * ship2_translate, cam*proj);
	
	GsMat redShip1_scale, redShip1_translate;
	redShip1_scale.scale(.01f, .01f, .01f);
	redShip1_translate.translation(3 + sin(t*0.75f + .2f)*.1f, sin(t*1.23f + .2f)*.03f, 0);
	redShip1->draw(redShip1_scale * redShip1_translate, cam*proj);

	GsMat redBigGunShip_scale, redBigGunShip_translate;
	redBigGunShip_scale.scale(.01f, .01f, .01f);
	redBigGunShip_translate.translation(5 + sin(t*0.75f + .2f)*.1f, sin(t*1.23f + .2f)*.03f, 0);
	redBigGunShip->draw(redBigGunShip_scale * redBigGunShip_translate, cam*proj);

	scene->draw(GsMat(), cam*proj);

	geo->unbindFramebuffer();

	glViewport(0, 0, width, height);
	glClearColor(.5f, .5f, cos(t)*0.5f + 0.5f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // | GL_STENCIL_BUFFER_BIT

	scene->drawSky(GsMat(), camNoTranslate * proj);

	// ~~~ Deferred Lighting ~~~
	// Draw in ambient:
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	DeferredUtils::addLight(Light(GsVec(0, .5f, 0), 1, RColor(.5f, .5f, 1, 1)));

	DeferredUtils::addLight(Light(GsVec(1, .5f, 0), 5, RColor(1, 1, 1, 1)));

	DeferredUtils::addLight(Light(GsVec(2, .5f, 0), 5, RColor(1, 1, 1, 1)));

	DeferredUtils::addLight(Light(GsVec(3, .5f, 0), 5, RColor(1, 1, 1, 1)));

	DeferredUtils::addLight(Light(GsVec(5, .5f, 0), 12, RColor(1, 1, 1, 1)));

	DeferredUtils::setGlobalLight(GlobalLight(GsVec(1, 1, 1), RColor(.1f, .1f, .1f, 1), RColor(.2f, .2f, .2f, 1)));
	DeferredUtils::applyGlobalLighting(*(geo->getTexture(0)), *(geo->getTexture(1)), *(geo->getTexture(2)));

	DeferredUtils::applyLighting(*(geo->getTexture(1)), *(geo->getTexture(2)), *(geo->getDepthTexture()), cam * proj);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	// COPY DEPTH BUFFER TO BACK BUFFER
	DeferredUtils::drawTextureToDepth(*(geo->getDepthTexture()));

	// ~~~ End deferred lighting ~~~
	
	// POST DRAWING

	// TODO: organize when to and when not to use blendmodes
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	arwing->addDraw(arwingRotY * arwingTr * arwingRot, cam * proj);
	scene->addDraw(GsMat(), cam *proj);
	redBigGunShip_lights->draw(redBigGunShip_scale * redBigGunShip_translate, cam*proj);

	ParticleUtils::draw(particles, cam, proj);

	//quad->draw(tr * billboard, cam * proj);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	if (showBuffers){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //  | GL_STENCIL_BUFFER_BIT

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


	
	/*
	glBindFramebuffer(GL_READ_FRAMEBUFFER, geo->frameBufId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, geo->w, geo->h,
					  0, 0, width/2, height/2, 
					  GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glReadBuffer(GL_DEPTH_ATTACHMENT);
	glBlitFramebuffer(0, 0, geo->w, geo->h,
		width / 2, 0, width, height / 2,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/
}