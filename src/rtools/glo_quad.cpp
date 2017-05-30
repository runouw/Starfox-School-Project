#include "glo_quad.h"

void GLO_Quad::init(const GLProgram& prog){
	setProgram(prog);
	numVertices = 12;

	// Do I still need this VBA?
	va = 0;
	gen_vertex_arrays(1); // vba
	// ^^ ??

	genBuffers(3); // x,y,z; nx,ny,nz, uvx, uvy; 

	float* _p = new float[numVertices * 3]; // positions
	float* p = _p;

	float* _n = new float[numVertices * 3]; // normals
	float* n = _n;

	float* _uvs = new float[numVertices * 2]; // uvs
	float* uvs = _uvs;

	*(p++) = -.5f; *(p++) = -.5f; *(p++) = 0;
	*(uvs++) = 0; *(uvs++) = 0;

	*(p++) = .5f; *(p++) = -.5f; *(p++) = 0;
	*(uvs++) = 1; *(uvs++) = 0;

	*(p++) = .5f; *(p++) = .5f; *(p++) = 0;
	*(uvs++) = 1; *(uvs++) = 1;

	*(p++) = -.5f; *(p++) = .5f; *(p++) = 0;
	*(uvs++) = 0; *(uvs++) = 1;

	// uploading to buffers:

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), _p, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), _n, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(float), _uvs, GL_STATIC_DRAW);

	delete _p;
	delete _n;
	delete _uvs;

	// Set up VAO:
	glBindVertexArray(va[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // uvs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	numIndices = 6;

	indices = new int[numIndices];
	int *_indices = indices;

	*(_indices++) = 0;
	*(_indices++) = 1;
	*(_indices++) = 2;
	*(_indices++) = 0;
	*(_indices++) = 2;
	*(_indices++) = 3;
}
void GLO_Quad::draw(const GsMat& tr, const GsMat& pr){
	program.bind();
	program.setUniformMat4f("vTransf", tr.e);
	program.setUniformMat4f("vProj", pr.e);

	tex->bind(GL_TEXTURE0);
	program.setUniform1i("Texture0", 0);

	glBindVertexArray(va[0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

	glBindVertexArray(0);

	// program.unbind();
}