#pragma once

#include <gltools\gltools.h>

class Scene{
public:
	float distance;

	GsMat resultMat;

	virtual void init() = 0;
	virtual void advance(float distance) = 0;

	virtual void drawSky(const GsMat& tr, const GsMat& pr) = 0;
	virtual void draw(const GsMat& tr, const GsMat& pr) = 0;
	virtual void postDraw(const GsMat& tr, const GsMat& pr) = 0;
	virtual void addDraw(const GsMat& tr, const GsMat& pr) = 0;

	virtual float intersect(const GsVec& start, const GsVec& end) = 0;
};