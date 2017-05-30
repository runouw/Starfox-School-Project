#pragma once

#include <GL/glew.h>
#include <freeglut.h>

#include <rtools\ai\shaders.h>
#include <rtools\screenquad.h>
#include <rtools\utils.h>

# include <gltools/gltools.h>

# include <gsim/gs_color.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_image.h>

#define MAX_NUM_LIGHTS 1000

using namespace GLTools;



struct Light{
	GsVec pos;
	float radius;
	RColor color;

	Light(GsVec pos, float radius, RColor color = RColor::WHITE) :pos(pos), radius(radius), color(color){ }
};
struct GlobalLight{
	GsVec dir;
	RColor ambient;
	RColor diffuse;
	GlobalLight(GsVec dir, RColor ambient, RColor diffuse): dir(dir), ambient(ambient), diffuse(diffuse) {}

	const static GlobalLight DEFUALT;
};

class LightSphere{
private:
	int na, nb;

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
	GLuint *va, *buf;

	int numVertices;
	int numIndices;

	int* indices;

	void gen(float radius, int subdivisions);
	void draw(int num, float* positions, float* colors);
};

class DeferredUtils{
	static GlobalLight globalLight;

	static int numLights;
	static float* positions;
	static float* colors;
	static LightSphere* sphere;

	static ScreenQuad* screenQuad;

public:
	static void init(float genRadius, int genVertices);
	static void addLight(Light& light);

	static void setGlobalLight(GlobalLight globalLight);
	static void applyGlobalLighting(GLTexture& ambient, GLTexture& diffuse, GLTexture& normals);
	static void applyLighting(GLTexture& diffuse, GLTexture& normals, GLTexture& depth, const GsMat& proj);

	static void drawTextureToDepth(GLTexture& tex);
};