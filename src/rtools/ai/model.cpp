#include <rtools\ai\model.h>

Model::Model(const aiScene* scene, const aiNode* node) : scene(scene), node(node){
	models = std::vector<Model*>();
	meshes = std::vector<Mesh*>();

	aiMatrix4x4 m = node->mTransformation;
	aiTransposeMatrix4(&m); // update transform (is this needed?)
	mat.set((float*) (&m));

	// draw all meshes assigned to this node
	for (std::size_t n = 0; n < (node->mNumMeshes); n++) {
		const struct aiMesh* aiMesh = scene->mMeshes[node->mMeshes[n]];

		Mesh* mesh = new Mesh(scene, aiMesh);
		meshes.push_back(mesh);

	}

	// draw all children
	for (std::size_t n = 0; n < node->mNumChildren; n++) {
		const struct aiNode* aiNode = node->mChildren[n];
		Model* node = new Model(scene, aiNode);

		models.push_back(node);
	}
}

void Model::init(){
	for (std::size_t i = 0; i < meshes.size(); i++){
		meshes[i]->init();
	}
	for (std::size_t i = 0; i < models.size(); i++){
		models[i]->init();
	}
}

void Model::draw(const GsMat& tr, const GsMat& pr){
	GsMat tr2 = tr * mat;


	std::size_t l;
	l = meshes.size();
	for (std::size_t i = 0; i < l; i++){
		meshes[i]->draw(tr2, pr);
	}
	l = models.size();
	for (std::size_t i = 0; i < l; i++){
		models[i]->draw(tr2, pr);
	}
}

float Model::intersect(const GsVec& start, const GsVec& end){
	float min = -1;

	std::size_t l;
	l = meshes.size();
	for (std::size_t i = 0; i < l; i++){
		float val = meshes[i]->intersect(start, end);
		if (val >= 0){
			if (min < 0 || val < min){
				min = val;
			}
		}
	}
	l = models.size();
	for (std::size_t i = 0; i < l; i++){
		float val = models[i]->intersect(start, end);
		if (val >= 0){
			if (min < 0 || val < min){
				min = val;
			}
		}
	}

	return min;
}
