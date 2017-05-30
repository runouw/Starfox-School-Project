#include "screenquad.h"

void ScreenQuad::gen(){
	numVertices = 4;

	va = 0;
	buf = 0;
	gen_vertex_arrays(1); // sphere's vba
	gen_buffers(2); // x,y,z; uvx, uvy; 

	float* _p = new float[numVertices * 3]; // positions
	float* p = _p;

	float* _uvs = new float[numVertices * 2]; // uvs
	float* uvs = _uvs;

	*(p++) = -1; *(p++) = -1; *(p++) = 0;
	*(p++) = 1; *(p++) = -1; *(p++) = 0;
	*(p++) = 1; *(p++) = 1; *(p++) = 0;
	*(p++) = -1; *(p++) = 1; *(p++) = 0;

	*(uvs++) = 0; *(uvs++) = 0;
	*(uvs++) = 1; *(uvs++) = 0;
	*(uvs++) = 1; *(uvs++) = 1;
	*(uvs++) = 0; *(uvs++) = 1;


	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), _p, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(float), _uvs, GL_STATIC_DRAW);

	delete _p;
	delete _uvs;

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // uvs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

}
void ScreenQuad::draw(){
	glBindVertexArray(va[0]);

	glDrawArrays(GL_QUADS, 0, numVertices);

	glBindVertexArray(0);
}