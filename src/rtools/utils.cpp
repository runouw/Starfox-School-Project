#include "utils.h"

const RColor RColor::WHITE = RColor(1, 1, 1, 1);

GLProgram* RTools::compileShader(const char* vert, const char* frag){
	GLShader vertShader;
	GLShader fragShader;

	GLProgram* prog = new GLProgram();

	vertShader.loadAndCompile(GL_VERTEX_SHADER, vert);
	fragShader.loadAndCompile(GL_FRAGMENT_SHADER, frag);
	prog->initAndLink(vertShader, fragShader);

	return prog;
}

GsMat RTools::billboardify(GsMat mat){
	mat.e[0] = 1; mat.e[1] = 0; mat.e[2] = 0;
	mat.e[4] = 0; mat.e[5] = 1; mat.e[6] = 0;
	mat.e[8] = 0; mat.e[9] = 0; mat.e[10] = 1;

	return mat;
}