#pragma once

#include <rtools\ai\model.h>
#include <rtools\ai\mesh.h>
#include <rtools\ai\material.h>

#include <game\models\arwing.h>

using namespace GLTools;

#define RETICLE_DISTANCE 5

class Player;

class Player{
	Arwing* arwing;
	
	GsMat transform;

	GLO_Quad* cursorQuad;
public:
	float x;
	float y;

	float px;
	float py;

	float vx;
	float vy;

	float cvx;
	float cvy;

	float tilt;
	float tiltV;

	// free half width and half height
	float freeHW;
	float freeHH;

	Player();
	void init();

	void idle(float dt);
	void draw(const GsMat& tr, const GsMat& pr);
	void addDraw(const GsMat& tr, const GsMat& pr);

	void drawCursor(const GsMat& tr, const GsMat& pr);

	virtual float lineIntersect(const GsVec& start, const GsVec& end);
};