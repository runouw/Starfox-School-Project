#include "textures.h"

static Textures* singleton = 0;

Textures::Textures(){
	singleton = this;
}

GLTexture* Textures::_get(std::string url){
	//url = "C:\\Users\\Robert\\Documents\\Projects svn\\OOP Proj\\Attempt2 (dont reinvent the wheel)\\OOPFinalProject\\res\\models\\arwing\\starfox_144_warp_10.png";

	if (this->textures.find(url) == this->textures.end()){
		// load for the first time

		// Make a texture object:

		//GLuint texID;
		//glGenTextures(1, &texID);

		// THEN try to load texture...
		// ...
		// ...
		// ...

		GsImage img = GsImage();
		img.load(url.c_str());

		GLTexture* tex = new GLTexture();
		tex->load(img, true);
		
		// Add to map
		//GLTexture* tex = new GLTexture(texID);
		this->textures[url] = tex;
	}

	return this->textures[url];
}

GLTexture* Textures::get(std::string url){
	if (singleton){
		return singleton->_get(url);
	}else{
		printf("Error: class Textues has not been initialized!");
	}
	return NULL;
}

void Textures::_releaseAll(){
	std::map<std::string, GLTexture*>::iterator itr = textures.begin();

	while (itr != textures.end()){
		printf("Freeing texture: %s", (itr->first).c_str());

		itr->second->clear();

		delete itr->second;

		++itr;
	}

	textures.clear();
}
void Textures::releaseAll(){
	if (singleton){
		singleton->_releaseAll();
	}else{
		printf("Error: class Textues has not been initialized!");
	}
}