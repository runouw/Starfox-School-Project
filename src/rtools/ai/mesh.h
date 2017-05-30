#pragma once

#include <gltools\gltools.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <rtools\ai\material.h>

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

class Mesh{
	const struct aiScene* scene;
	const struct aiMesh* mesh;

	GLuint* buf;
	GLuint bCount;
	void setProgram(const GLProgram& prog);
	void genBuffers(GLuint n);

	int numPolygons;
	int numVertices;

	GLuint vao;

	GsVec boundsMin, boundsMax;

	Material* material;
public:
	Mesh(const aiScene* scene, const struct aiMesh* mesh);

	void init();
	void draw(const GsMat& tr, const GsMat& pr);
	float intersect(const GsVec& start, const GsVec& end);

	static void setDefaultMaterial(Material*);
};
