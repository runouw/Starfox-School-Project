#pragma once

#include <GL/glew.h>
#include <freeglut.h>

#include <gltools/gltools.h>
#include <rtools\utils.h>

using namespace GLTools;

// SINGLETON CLASS for texture management. If the same texture (by filename) is loaded twice, the texture will not have to re-load
class Shaders{
private:
	
public:
	Shaders();

	static void init();

	static GLProgram* getSimpleShader();
	static GLProgram* getDeferredShader();
	static GLProgram* getDeferredLightShader();
	static GLProgram* getDeferredGlobalShader();
	static GLProgram* getReticleShader();
	static GLProgram* getLaserShader();
	static GLProgram* getNebulaShader();

	static GLProgram* getDepthShader();
	static GLProgram* getParticleShader();
};