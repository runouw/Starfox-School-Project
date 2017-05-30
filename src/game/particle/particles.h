#pragma once

#include <map>
#include <vector>

#include <gltools\gltools.h>

#include <rtools\glo_quad.h>
#include <rtools\utils.h>

#include <GL/glew.h>
#include <freeglut.h>

# include <gsim/gs_color.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_image.h>

using namespace GLTools;

#define MAX_PARTICLES 100000

class ParticleQuad{
private:
	GLuint *va, *buf;
	int na, nb;

	int numVertices;

	void gen_vertex_arrays(GLsizei n) {
		delete[] va;
		va = new GLuint[n];
		glGenVertexArrays(n, va);
		na = (gsbyte)n;
	}
	void gen_buffers(GLsizei n) {
		delete[] buf;
		buf = new GLuint[n];
		glGenBuffers(n, buf);
		nb = (gsbyte)n;
	}

public:
	void gen();
	void draw(int num, float* positions, float* colors);
};


// Billboarded Front Facing Particle
class Particle{

public:
	bool exists; // set to false to be removed from array (and the destructor will be called)
	bool del; // set to false when you are handling memory yourself
	
	GsVec pos;
	float size;
	RColor color;
	GLTexture* tex;
	float rotation;

	Particle(){
		exists = true;
		del = true;
		rotation = gs_random() * (3.15159f * 2.0f);
	};
	Particle(GLTexture* tex, GsVec pos, float size, RColor color);

	virtual void idle(float dt) = 0;
};

class Particles{
public:
	// TODO: maps for different kinds of particles...
	std::map<int, std::vector<Particle*>*> map;

	Particles(){
		map = std::map<int, std::vector<Particle*>*>();
	}
};

class ParticleUtils{
public:
	static ParticleQuad quad;

	static float* positions;
	static float* colors;

	static void init();

	static void addParticle(Particles &particles, Particle* particle);
	static std::vector<Particle*>* getParticleList(Particles &particles, int texID);

	static void idle(Particles &particles, float dt);
	static void draw(Particles &particles, const GsMat& cam, const GsMat& proj);
};

class Particle_Simple : public Particle{
public:
	GsVec vel;
	float duration;
	float t;

	Particle_Simple(GLTexture* tex, GsVec pos, float size, RColor color, GsVec vel, float duration);
	void idle(float dt);
};

class Particle_SimpleDecay : public Particle{
public:
	GsVec vel;
	float duration;
	float t;
	float initialSize;

	Particle_SimpleDecay(){};
	Particle_SimpleDecay(GLTexture* tex, GsVec pos, float size, RColor color, GsVec vel, float duration);
	void idle(float dt);
};

class Particle_ExplosionDecay : public Particle{
public:
	GsVec vel;
	float duration;
	float t;
	float initialSize;
	float rotSpeed;

	Particle_ExplosionDecay(){};
	Particle_ExplosionDecay(GLTexture* tex, GsVec pos, float size, RColor color, GsVec vel, float duration, float rotSpeed);
	void idle(float dt);
};