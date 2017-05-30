#include "scene_asteroid1.h"

#include <game\engine.h>

float enemySpawn;
float enemySpawn2;
bool spawnBoss;
float levelSpeed;

Scene_Asteroid1::Scene_Asteroid1(){
	distance = 0;
	enemySpawn = 0;
	enemySpawn2 = 0;
	levelSpeed = 2.5f;

	resultMat = GsMat();
	spawnBoss = false;
}

void Scene_Asteroid1::init(){
	Material::setDefaultProgram(Shaders::getDeferredShader());
	
	
	{
		const struct aiScene* scene;

		// to load textures from correct location:
		Material::setBasePath("../res/models/scene/asteroids/");
		scene = aiImportFile("../res/models/scene/asteroids/some_asteroids.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!");
		}

		// extract model from OBJ:
		asteroids = new Model(scene, scene->mRootNode);
		asteroids->init();
	}

	
	{
		Material::setDefaultProgram(Shaders::getSimpleShader());
		const struct aiScene* scene;

		// to load textures from correct location:
		Material::setBasePath("../res/models/sky1/");
		scene = aiImportFile("../res/models/sky1/starfield.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!");
		}

		// extract model from OBJ:
		starfield = new Model(scene, scene->mRootNode);
		starfield->init();
	}
	
	{
		Material::setDefaultProgram(Shaders::getNebulaShader());

		const struct aiScene* scene;

		// to load textures from correct location:
		Material::setBasePath("../res/models/sky1/");
		scene = aiImportFile("../res/models/sky1/nebula.obj", aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene){
			// TODO: quit?
			printf("OBJ failed to load!");
		}

		// extract model from OBJ:
		nebula = new Model(scene, scene->mRootNode);
		nebula->init();

		Material::setDefaultProgram(Shaders::getSimpleShader());
	}

	printf("Scene loading complete!\n");
	fflush(stdout);

}
void Scene_Asteroid1::advance(float d){
	distance += d * levelSpeed;

	float rotDown = 0;
	float ty = 0;
	float tx = 0;

	float at1 = 40;

	if ((distance - at1) < 0){
		
	}else{
		if ((distance - at1) < 20){
			ty += RTools::easeInOutQuad((distance - at1), 0, -10, 20);

			if ((distance - at1) < 10){
				rotDown += -RTools::easeInOutQuad((distance - at1), 0, .25f, 10);
			}
			else{
				rotDown += -RTools::easeInOutQuad((distance - (at1 + 10)), .25f, -.25f, 10);
			}
		}
		else{
			ty += -10;
		}
	}

	float at2 = 80;

	if ((distance - at2) < 0){

	}
	else{
		if ((distance - at2) < 40){
			ty += RTools::easeInOutQuad((distance - at2), 0, 15, 40);

			levelSpeed = 2.5f + RTools::easeInOutQuad((distance - at2), 0, 4, 40);

			if ((distance - at2) < 20){
				rotDown += -RTools::easeInOutQuad((distance - at2), 0, -.3f, 20);
			}else{
				rotDown += -RTools::easeInOutQuad((distance - (at2 + 20)), -.3f, .3f, 20);
			}
		}
		else{
			ty += 15;
		}
	}

	float at3 = 325;

	if ((distance - at3) < 0){

	}
	else{
		if ((distance - at3) < 40){

			ty += RTools::easeInOutQuad((distance - at3), 0, 15, 40);
			tx += RTools::easeInOutQuad((distance - at3), 0, 15, 40);

			if ((distance - at3) < 20){
				rotDown += -RTools::easeInOutQuad((distance - at3), 0, -.3f, 20);
			}
			else{
				rotDown += -RTools::easeInOutQuad((distance - (at3 + 20)), -.3f, .3f, 20);
			}
		}
		else{
			ty += 15;
			tx += 15;
		}
	}

	if (distance > 420){
		levelSpeed = 2.5f;
	}

	GsMat translateMat, rotateUp;

	translateMat.translation(tx, ty, distance);

	rotateUp.rotx(rotDown);

	resultMat = translateMat * rotateUp;

	// ENEMIES
	if (distance > 10 && distance < 46){

		enemySpawn2 += d;
		while (enemySpawn2 > .4f){
			enemySpawn2 -= 2.4f;

			float rndx = (float)(gs_randomd() * 2 - 1) * 5.0f;
			float rndy = (float)(gs_randomd() * 2 - 1) * 5.0f;

			float rndz = (float)gs_randomd() * 3.0f;

			float rndrx = (float)(gs_randomd() * 2 - 1) * .1f;
			float rndry = (float)(gs_randomd() * 2 - 1) * .1f;

			GsVec dir = GsVec(rndrx, rndry, 1);
			dir.normalize();

			Engine::enemies.push_back(new Enemy_Ship2(GsVec(rndx, rndy + 15, -100 - rndz - distance), dir));
		}
	}


	if (distance > 40 && distance < 46){

		enemySpawn += d;
		while (enemySpawn > .4f){
			enemySpawn -= 0.4f;

			float rndx = (float)(gs_randomd() * 2 - 1) * 5.0f;
			float rndy = (float)(gs_randomd() * 2 - 1) * 5.0f;

			float rndz = (float)gs_randomd() * 3.0f;

			float rndrx = (float)(gs_randomd() * 2 - 1) * .1f;
			float rndry = (float)(gs_randomd() * 2 - 1) * .1f;

			GsVec dir = GsVec(rndrx, rndry, 1);
			dir.normalize();

			Engine::enemies.push_back(new Enemy_Ship1(GsVec(rndx, rndy + 15, -60 - rndz - distance), dir));
		}
	}

	if (distance > 80 && distance < 220){

		enemySpawn += d;
		while (enemySpawn > .5f){
			enemySpawn -= 1.5f;

			float rndx = (float)(gs_randomd() * 2 - 1) * 5.0f;
			float rndy = (float)(gs_randomd() * 2 - 1) * 5.0f;

			float rndz = (float)gs_randomd() * 3.0f;

			float rndrx = (float)(gs_randomd() * 2 - 1) * .1f;
			float rndry = (float)(gs_randomd() * 2 - 1) * .1f;

			GsVec dir = GsVec(rndrx, rndry, 1);
			dir.normalize();

			Engine::enemies.push_back(new Enemy_Ship1(GsVec(rndx, rndy, -100 - rndz - distance), dir));
		}
	}
	if (distance > 40 && distance < 220){

		enemySpawn2 += d;
		while (enemySpawn2 > .4f){
			enemySpawn2 -= 2.4f;

			float rndx = (float)(gs_randomd() * 2 - 1) * 5.0f;
			float rndy = (float)(gs_randomd() * 2 - 1) * 5.0f;

			float rndz = (float)gs_randomd() * 3.0f;

			float rndrx = (float)(gs_randomd() * 2 - 1) * .1f;
			float rndry = (float)(gs_randomd() * 2 - 1) * .1f;

			GsVec dir = GsVec(rndrx, rndry, 1);
			dir.normalize();

			Engine::enemies.push_back(new Enemy_Ship2(GsVec(rndx, rndy - 4, -100 - rndz - distance), dir));
		}
	}

	if (distance > 220 && distance < 320){

		enemySpawn += d;
		while (enemySpawn > .5f){
			enemySpawn -= 1.5f;

			float rndx = (float)(gs_randomd() * 2 - 1) * 5.0f;
			float rndy = (float)(gs_randomd() * 2 - 1) * 5.0f;

			float rndz = (float)gs_randomd() * 3.0f;

			float rndrx = (float)(gs_randomd() * 2 - 1) * .1f;
			float rndry = (float)(gs_randomd() * 2 - 1) * .1f;

			GsVec dir = GsVec(rndrx, rndry, 1);
			dir.normalize();

			Engine::enemies.push_back(new Enemy_Ship1(GsVec(rndx, rndy - 10, -100 - rndz - distance), dir));
		}
	}
	if (distance > 220 && distance < 320){

		enemySpawn2 += d;
		while (enemySpawn2 > .4f){
			enemySpawn2 -= 2.4f;

			float rndx = (float)(gs_randomd() * 2 - 1) * 5.0f;
			float rndy = (float)(gs_randomd() * 2 - 1) * 5.0f;

			float rndz = (float)gs_randomd() * 3.0f;

			float rndrx = (float)(gs_randomd() * 2 - 1) * .1f;
			float rndry = (float)(gs_randomd() * 2 - 1) * .1f;

			GsVec dir = GsVec(rndrx, rndry, 1);
			dir.normalize();

			Engine::enemies.push_back(new Enemy_Ship2(GsVec(rndx, rndy - 10, -100 - rndz - distance), dir));
		}
	}


	if (distance > 350){
		if (spawnBoss == false){
			spawnBoss = true;
			// Spawn boss:

			Engine::enemies.push_back(new Boss_BigGun(GsVec(-25, 0, -150 - distance), GsVec(0, 0, 1)));
		}
	}
}

void Scene_Asteroid1::drawSky(const GsMat& tr, const GsMat& pr){
	GsMat mat = resultMat * tr;
	mat.set(12, 0);
	mat.set(13, 0);
	mat.set(14, 0);

	glDepthMask(GL_FALSE);
	starfield->draw(mat, pr);
	nebula->draw(mat, pr);
	glDepthMask(GL_TRUE);
}
void Scene_Asteroid1::draw(const GsMat& tr, const GsMat& pr){
	asteroids->draw(tr, pr);
}
void Scene_Asteroid1::postDraw(const GsMat& tr, const GsMat& pr){

}
void Scene_Asteroid1::addDraw(const GsMat& tr, const GsMat& pr){

}
float Scene_Asteroid1::intersect(const GsVec& start, const GsVec& end){
	return asteroids->intersect(start, end);
}