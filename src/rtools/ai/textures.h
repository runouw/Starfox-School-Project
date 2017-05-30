#pragma once

#include <GL/glew.h>
#include <freeglut.h>

# include <gltools/gltools.h>

#include <string>
#include <map>

using namespace GLTools;

// SINGLETON CLASS for texture management. If the same texture (by filename) is loaded twice, the texture will not have to re-load
class Textures{
private:
	// URL to openGL tex id hash map
	std::map<std::string, GLTexture*> textures;

	void _releaseAll();
	GLTexture* _get(std::string url);
public:
	// Constructor to initialize singleton
	Textures();

	// Get texture by URL. Returns a GLOOP (GLTools) GLTexture
	static GLTexture* get(std::string url);

	// Too lazy to implement fast resource management, so i'll release all textures and load all between every level (OS level file caching is good enough, right?)
	static void releaseAll();
};