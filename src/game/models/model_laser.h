#pragma once

#include <rtools\ai\model.h>
#include <rtools\ai\mesh.h>
#include <rtools\ai\material.h>
#include <rtools\ai\shaders.h>
#include <rtools\utils.h>

#include <rtools\glo_quad.h>

using namespace GLTools;


// A simple laser drawn with quads
// You may set the color diameter, direction & length of it.
class Model_Laser{
	static GLO_Quad* quad;
public:

	static void init();
	static void addDraw(const GsMat& tr, const GsMat &pr, const RColor& color, const GsVec& pos, const GsVec& dir, float diameter);
};