#include <rtools\ai\material.h>

GLProgram* defaultProgram;
std::string basePath;

void Material::setDefaultProgram(GLProgram* pr){
	defaultProgram = pr;
}
void Material::setBasePath(std::string str){
	basePath = str;
}

Material::Material(){

}
Material::Material(const aiMaterial *mtl){
	program = defaultProgram;

	int texIndex = 0;
	aiString texPath;

	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)){
		tex = Textures::get(basePath + texPath.data);
	}
	// TODO: more kinds of materials

}
