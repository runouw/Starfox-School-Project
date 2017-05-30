#pragma once

// Class that handles the projectiles
// Communicates with the Engine class to get the player and enemies
// The enemy list (std::vector) is located in Engine::enemies

#include <vector>
#include <gsim\gs_vec.h>
#include <gsim\gs_mat.h>

#include <rtools\glo_quad.h>

class Projectile{

public:
	bool exists; // set to false to be removed from array (and the destructor will be called)

	virtual void idle(float dt) = 0;

	virtual void draw(const GsMat& tr, const GsMat& pr) = 0;
	virtual void postDraw(const GsMat& tr, const GsMat& pr) = 0;
	virtual void addDraw(const GsMat& tr, const GsMat& pr) = 0;
	virtual void putLights() = 0;
};

// Laser class. Should this include ALL types of lasers?
class Laser : public Projectile{
	GsVec pos;
	GsVec dir;
	float vel;

	float life;
public:

	Laser(const GsVec p, const GsVec dir);

	void idle(float dt);

	void draw(const GsMat& tr, const GsMat& pr){};
	void postDraw(const GsMat& tr, const GsMat& pr){};
	void addDraw(const GsMat& tr, const GsMat& pr);
	void putLights();
};
