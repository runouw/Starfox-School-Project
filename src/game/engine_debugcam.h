#pragma once

#include "engine.h"

#include <rtools/glactivity.h>
#include <GL/glew.h>
#include <freeglut.h>
#include <cmath>

#include <gltools\gltools.h>
#include <gsim\gs_image.h>
#include <rtools\glo_quad.h>
#include <rtools\ai\textures.h>
#include <rtools\ai\model.h>
#include <rtools\keys.h>
#include <game\models\arwing.h>

#include <rtools\ai\model.h>
#include <rtools\ai\mesh.h>
#include <rtools\ai\material.h>
#include <rtools\ai\shaders.h>
#include <game\models\arwing.h>

#include <game\scene\scene.h>

#include <game\player.h>

#define ASSIMP_BUILD_BOOST_WORKAROUND
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <game\scene\scene_asteroid1.h>

#include <rtools\post\deferred.h>

class Engine_DebugCam : public Engine{
	Arwing* arwing;

	bool dragCam;
	int oldmx, oldmy;

	float camRotX, camRotY;
	float cx, cy, cz;
	float vx, vy, vz;

	GLenum* geoFormats;
	GLGeometrybuffer* geo;

	GLO_Quad* quad;

	bool showBuffers;

protected:
	virtual void mouseDown(int button);
	virtual void mouseUp(int button);
	virtual void changedWH();
	virtual void idle(float dt);
	virtual void render();
	
public:
	Engine_DebugCam();
	virtual void init(int w, int h);
};