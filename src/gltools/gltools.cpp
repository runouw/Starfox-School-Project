# include "gltools.h"
# include <fstream>
# include <iostream>
# include <string>

GLTools::GLLight::GLLight(){
#define _SETC(E, r, g, b, a) E[0] = r; E[1] = g; E[2] = b; E[3] = a
	_SETC(this->ambient.e, 0, 0, 0, 0);
	_SETC(this->diffuse.e, 1.0f, 1.0f, 1.0f, 1.0f);
	_SETC(this->specular.e, 0, 0, 0, 0);
#undef _SETC
}
GLTools::GLMaterial::GLMaterial(){
#define _SETC(E, r, g, b, a) E[0] = r; E[1] = g; E[2] = b; E[3] = a
	_SETC(this->ambient.e, 0, 0.0, 0.0, 0.0);
	_SETC(this->diffuse.e, 1.0f, 1.0f, 1.0f, 1.0f);
	_SETC(this->specular.e, 0, 0, 0, 0);
	this->shininess = 0.0f;
#undef _SETC
}