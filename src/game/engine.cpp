#include <game\engine.h>

using namespace GLTools;

float Engine::t = 0;
GsMat Engine::cam = GsMat();
GsMat Engine::camAntiRot = GsMat();

Player* Engine::player = NULL;
std::vector<Enemy*> Engine::enemies = std::vector<Enemy*>();
std::vector<Projectile*> Engine::projectiles = std::vector<Projectile*>();
Particles Engine::particles = Particles();
Scene* Engine::scene = NULL;

float Engine::getTimer(){
	return t;
}
GsMat Engine::getCam(){
	return cam;
}
GsMat Engine::getCamAntiRot(){
	return camAntiRot;
}

GsVec Engine::getPlayerPosition(){
	GsVec ppos = GsVec(player->x, player->y, 0);
	ppos = ppos * (scene->resultMat).inverse();
	return ppos;
}