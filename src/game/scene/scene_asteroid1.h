#pragma once

#include "scene.h"

#include <gltools\gltools.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <rtools\ai\model.h>
#include <rtools\ai\mesh.h>
#include <rtools\ai\material.h>
#include <rtools\ai\textures.h>
#include <rtools\ai\shaders.h>

class Scene_Asteroid1 : public Scene{
	Model* asteroids;
	Model* starfield;
	Model* nebula;
public:
	Scene_Asteroid1();

	virtual void init();
	virtual void advance(float distance);

	virtual void drawSky(const GsMat& tr, const GsMat& pr);
	virtual void draw(const GsMat& tr, const GsMat& pr);
	virtual void postDraw(const GsMat& tr, const GsMat& pr);
	virtual void addDraw(const GsMat& tr, const GsMat& pr);

	virtual float intersect(const GsVec& start, const GsVec& end);
};