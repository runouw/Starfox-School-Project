#include "arwing.h"

#include <game\engine.h>

Arwing::Arwing(){
	wingTilt = 0;

	const struct aiScene* scene;

	Material::setDefaultProgram(Shaders::getDeferredShader());

	// to load textures from correct location:
	Material::setBasePath("../res/models/arwing/");
	scene = aiImportFile("../res/models/arwing/arwing_base.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene){
		// TODO: quit?
		printf("OBJ failed to load!");
	}

	// extract model from OBJ:
	arwing_base = new Model(scene, scene->mRootNode);


	scene = aiImportFile("../res/models/arwing/arwing_leftwing.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene){
		// TODO: quit?
		printf("OBJ failed to load!");
	}

	// extract model from OBJ:
	arwing_leftwing = new Model(scene, scene->mRootNode);


	scene = aiImportFile("../res/models/arwing/arwing_rightwing.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene){
		// TODO: quit?
		printf("OBJ failed to load!");
	}

	// extract model from OBJ:
	arwing_rightwing = new Model(scene, scene->mRootNode);

	Material::setDefaultProgram(Shaders::getSimpleShader());

	scene = aiImportFile("../res/models/arwing/arwing_jet.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene){
		// TODO: quit?
		printf("OBJ failed to load!");
	}

	// extract model from OBJ:
	arwing_jet = new Model(scene, scene->mRootNode);

	jetQuad = new GLO_Quad(Textures::get("../res/models/arwing/starfox_147_GfoxJet.bmp"));
	
}

void Arwing::init(){
	arwing_base->init();
	arwing_leftwing->init();
	arwing_rightwing->init();
	arwing_jet->init();

	jetQuad->init(*Shaders::getSimpleShader());
}
void Arwing::draw(const GsMat& tr, const GsMat& pr){
	float t = Engine::getTimer();

	GsMat fixMiddle;
	fixMiddle.translation(0, -.06f, 0);

	GsMat tilt1, tilt2;
	tilt1.rotz(wingTilt);
	tilt2.rotz(-wingTilt);

	GsMat wing1tr1, wing1tr2;
	wing1tr1.translation(-.18f, 0, 0);
	wing1tr2.translation(.18f, 0, 0);

	GsMat wing2tr1, wing2tr2;
	wing2tr1.translation(.18f, 0, 0);
	wing2tr2.translation(-.18f, 0, 0);

	arwing_base->draw(fixMiddle * tr, pr);
	arwing_leftwing->draw((wing1tr1 * tilt1 * wing1tr2) * fixMiddle * tr, pr);
	arwing_rightwing->draw((wing2tr1 * tilt2 * wing2tr2) * fixMiddle * tr, pr);
}

void Arwing::addDraw(const GsMat& tr, const GsMat& pr){
	float t = Engine::getTimer();

	GsMat scaleJet, moveJet, scaleQuad, moveQuad;
	moveJet.translation(0, .04f - .06f, -.27f);
	moveQuad.translation(0, 0, -.04f);

	float s = 1.0f + sin(t * 12)*.1f;
	scaleJet.scale(s, s, s);
	scaleQuad.scale(.15f, .15f, .15f);

	
	arwing_jet->draw(scaleJet * moveJet * tr, pr);

	jetQuad->draw(scaleJet * scaleQuad * Engine::getCamAntiRot() * RTools::billboardify(moveJet * moveQuad * tr), pr);
}

float Arwing::lineIntersect(const GsVec& start, const GsVec& end){
	GsMat fixMiddle;
	fixMiddle.translation(0, -.06f, 0);

	GsMat tilt1, tilt2;
	tilt1.rotz(wingTilt);
	tilt2.rotz(-wingTilt);

	GsMat wing1tr1, wing1tr2;
	wing1tr1.translation(-.18f, 0, 0);
	wing1tr2.translation(.18f, 0, 0);

	GsMat wing2tr1, wing2tr2;
	wing2tr1.translation(.18f, 0, 0);
	wing2tr2.translation(-.18f, 0, 0);

	float min = -1;
	{
		//arwing_base->draw(fixMiddle * tr, pr);
		GsVec _start = start * (fixMiddle).inverse();
		GsVec _end = end * (fixMiddle).inverse();
		float dis = arwing_base->intersect(_start, _end);
		if (dis >= 0){
			if (min < 0 || dis < min){
				min = dis;
			}
		}
	}
	{
		//arwing_leftwing->draw((wing1tr1 * tilt1 * wing1tr2) * fixMiddle * tr, pr);
		GsVec _start = start * ((wing1tr1 * tilt1 * wing1tr2) * fixMiddle).inverse();
		GsVec _end = end * ((wing1tr1 * tilt1 * wing1tr2) * fixMiddle).inverse();
		float dis = arwing_leftwing->intersect(_start, _end);
		if (dis >= 0){
			if (min < 0 || dis < min){
				min = dis;
			}
		}
	}
	{
		//arwing_rightwing->draw((wing2tr1 * tilt2 * wing2tr2) * fixMiddle * tr, pr);
		GsVec _start = start * ((wing2tr1 * tilt2 * wing2tr2) * fixMiddle).inverse();
		GsVec _end = end * ((wing2tr1 * tilt2 * wing2tr2) * fixMiddle).inverse();
		float dis = arwing_rightwing->intersect(_start, _end);
		if (dis >= 0){
			if (min < 0 || dis < min){
				min = dis;
			}
		}
	}
	return min;
}