#pragma once

#include <GL/glew.h>
#include <freeglut.h>

# include <gsim/gs_color.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_image.h>

class ScreenQuad{
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
	void draw();
};
