#pragma once

#include <rtools/glactivity.h>
#include <GL/glew.h>
#include <freeglut.h>
#include <cmath>

#include <gltools/gltools.h>
#include <gsim/gs_image.h>
#include <rtools/glo_quad.h>
#include <rtools/ai/textures.h>
#include <rtools\keys.h>

#define ASSIMP_BUILD_BOOST_WORKAROUND
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <game\player.h>
#include <game\enemy\enemies.h>
#include <game\projectile\projectiles.h>
#include <game\particle\particles.h>
#include <game\scene\scene.h>

class Engine: public GLActivity{
protected:
	virtual void mouseDown(int button) = 0;
	virtual void mouseUp(int button) = 0;
	virtual void changedWH() = 0;
	virtual void idle(float dt) = 0;
	virtual void render() = 0;
public:
	virtual void init(int w, int h) = 0;

	static GsMat getCam();
	static GsMat getCamAntiRot();

	static float getTimer();

	static float t;

	static GsMat cam;
	static GsMat camAntiRot;

	static Player* player;

	static std::vector<Enemy*> enemies;
	static std::vector<Projectile*> projectiles;
	static Particles particles;

	static Scene* scene;

	static GsVec getPlayerPosition();

};
