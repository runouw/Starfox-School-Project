#include "model_laser.h"

GLO_Quad* Model_Laser::quad = NULL;

void Model_Laser::init(){
	quad = new GLO_Quad(Textures::get("../res/images/starfox_145_laser.bmp"));
	quad->init(*Shaders::getLaserShader());
}
void Model_Laser::addDraw(const GsMat& tr, const GsMat &pr, const RColor& color, const GsVec& pos, const GsVec& dir, float diameter){
	GLProgram* program = Shaders::getLaserShader();
	program->bind();
	program->setUniform4f("u_color", color.e);

	GsMat up, posMat, forward, mrx, mry, stretch;
	up.translation(0, -.5f, 0);
	posMat.translation((pos).e);

	float rx = atan2(dir.x, dir.z);
	float ry = atan2(dir.y, sqrt(dir.z * dir.z + dir.x * dir.x));

	forward.rotx(AI_MATH_HALF_PI_F);
	mrx.roty(rx + AI_MATH_PI_F);
	mry.rotx(ry);

	stretch.scale(diameter, dir.norm(), 1);

	quad->draw(tr * up * stretch * forward * mry * mrx * posMat, pr);
	GsMat rot;
	rot.rotz(AI_MATH_HALF_PI_F);

	quad->draw(tr * up * stretch * forward * rot * mry * mrx * posMat, pr);
}