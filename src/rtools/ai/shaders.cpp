#include "shaders.h"

static Shaders* singleton = 0;

GLProgram* simpleShader;
GLProgram* deferredShader;
GLProgram* deferredLightShader;
GLProgram* deferredGlobalShader;
GLProgram* reticleShader;
GLProgram* laserShader;
GLProgram* nebulaShader;

GLProgram* depthShader;
GLProgram* particleShader;

Shaders::Shaders(){
	singleton = this;
}

void Shaders::init(){
	simpleShader = RTools::compileShader("../res/shaders/simple/simple-tex.vert", "../res/shaders/simple/simple-tex.frag");
	deferredShader = RTools::compileShader("../res/shaders/deferred/deferred-tex.vert", "../res/shaders/deferred/deferred-tex.frag");
	deferredLightShader = RTools::compileShader("../res/shaders/deferred/deferred-post-light.vert", "../res/shaders/deferred/deferred-post-light.frag");
	deferredGlobalShader = RTools::compileShader("../res/shaders/deferred/deferred-post-global.vert", "../res/shaders/deferred/deferred-post-global.frag");
	reticleShader = RTools::compileShader("../res/shaders/player/reticle.vert", "../res/shaders/player/reticle.frag");
	laserShader = RTools::compileShader("../res/shaders/projectile/laser.vert", "../res/shaders/projectile/laser.frag");
	nebulaShader = RTools::compileShader("../res/shaders/sky/nebula.vert", "../res/shaders/sky/nebula.frag");
	depthShader = RTools::compileShader("../res/shaders/post/depth-tex.vert", "../res/shaders/post/depth-tex.frag");
	particleShader = RTools::compileShader("../res/shaders/particle/particle-simple.vert", "../res/shaders/particle/particle-simple.frag");
}

GLProgram* Shaders::getSimpleShader(){
	return simpleShader;
}

GLProgram* Shaders::getDeferredShader(){
	return deferredShader;
}

GLProgram* Shaders::getDeferredLightShader(){
	return deferredLightShader;
}

GLProgram* Shaders::getDeferredGlobalShader(){
	return deferredGlobalShader;
}

GLProgram* Shaders::getReticleShader(){
	return reticleShader;
}

GLProgram* Shaders::getLaserShader(){
	return laserShader;
}

GLProgram* Shaders::getNebulaShader(){
	return nebulaShader;
}

GLProgram* Shaders::getDepthShader(){
	return depthShader;
}

GLProgram* Shaders::getParticleShader(){
	return particleShader;
}