#pragma once

#include <gltools\gltools.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include <rtools\ai\mesh.h>

using namespace GLTools;

class Model {
protected:
	const struct aiScene* scene;
	const struct aiNode* node;
	std::vector<Model*> models;
	std::vector<Mesh*> meshes;

	GsMat mat;
public:
	Model(const aiScene* scene, const aiNode* node);

	void init();
	void draw(const GsMat& tr, const GsMat& pr);
	float intersect(const GsVec& start, const GsVec& end);
};