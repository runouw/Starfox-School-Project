#pragma once

#include <rtools\ai\model.h>
#include <rtools\ai\mesh.h>
#include <rtools\ai\material.h>
#include <rtools\ai\shaders.h>
#include <rtools\utils.h>

#include <rtools\glo_quad.h>

using namespace GLTools;

class Arwing{
	Model* arwing_base;
	Model* arwing_leftwing;
	Model* arwing_rightwing;

	Model* arwing_jet;

	GLO_Quad* jetQuad;

public:
	Arwing();

	float wingTilt;

	void init();
	void draw(const GsMat& tr, const GsMat &pr);

	//void postDraw(const GsMat& tr, const GsMat &pr);
	void addDraw(const GsMat& tr, const GsMat &pr);

	float lineIntersect(const GsVec& start, const GsVec& end);
};