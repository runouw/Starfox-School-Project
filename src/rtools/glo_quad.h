#pragma once

#include <GL/glew.h>
#include <freeglut.h>

# include <gltools/gltools.h>

# include <gsim/gs_color.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_image.h>

using namespace GLTools;

/*
class GLObject {
protected:
	GLProgram program;
	GLuint* buf;
	GLuint bCount;
	void setProgram(const GLProgram& prog);
	void genBuffers(GLuint n);
public:
	bool changed;
	GLObject();
	~GLObject();
	virtual void init(const GLProgram& prog){}
	virtual void draw(GsMat&, GsMat&){}
};
*/

class GLO_Quad : public GLObject{
private:
	GLuint *va;
	int na;

	int numVertices;
	int numIndices;

	int* indices;

	GLTexture* tex;

	void gen_vertex_arrays(GLsizei n) {
		delete[] va;
		va = new GLuint[n];
		glGenVertexArrays(n, va);
		na = (gsbyte)n;
	}

public:
	GLO_Quad(GLTexture* tex) : tex(tex) {};

	void setTexture(GLTexture* tex) { this->tex = tex; };

	virtual void init(const GLProgram& prog);
	virtual void draw(const GsMat&, const GsMat&);
};