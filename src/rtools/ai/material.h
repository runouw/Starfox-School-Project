#pragma once

#include <gltools\gltools.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <rtools\ai\textures.h>

using namespace GLTools;

// A single, not very conplicated material class.
// I don't really care about anything else than diffuse texture, so there's only one texture supported.
// Extend this class for more complicated material behavior...

class Material{
public:
	GLProgram* program;
	GLTexture* tex;

	Material();
	Material(const aiMaterial *mtl);

	static void setDefaultProgram(GLProgram*);
	static void setBasePath(std::string);
};