#include "deferred.h"

const float  PI_F = 3.14159265358979f;

const GlobalLight GlobalLight::DEFUALT = GlobalLight(GsVec(1, 1, 1), RColor(.2f, .2f, .2f, 1), RColor(.8f, .8f, .8f, 1));

void LightSphere::gen(float radius, int nfaces){
	numVertices = nfaces*nfaces;

	va = 0;
	buf = 0;
	gen_vertex_arrays(1); // sphere's vba
	gen_buffers(4); // x,y,z; nx,ny,nz; positions, colors

	float* _p = new float[numVertices * 3]; // positions
	float* p = _p;

	float* _n = new float[numVertices * 3]; // normals
	float* n = _n;

	for (int i = 0; i<nfaces; i++){
		float theta = (i / (float)nfaces)*(float)PI_F*2.0f;
		for (int h = 0; h<nfaces; h++){
			float phi = (h / (float)(nfaces - 1))*(float)PI_F + PI_F / 2.0f;

			float x = sin(theta)*radius;
			float z = cos(theta)*radius;

			float y = sin(phi)*radius;

			x *= cos(phi);
			z *= cos(phi);

			*(p++) = x;
			*(p++) = y;
			*(p++) = z;

			float nx = sin(theta);
			float nz = cos(theta);

			float ny = sin(phi);

			*(n++) = nx;
			*(n++) = ny;
			*(n++) = nz;
		}
	}

	glBindVertexArray(va[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);  // positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), _p, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);  // normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), _n, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // x,y,z, radius
	//glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, buf[3]); // r, g, b, a
	//glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(3, 1);

	delete _p;
	delete _n;

	glBindVertexArray(0);

	numIndices = nfaces*nfaces * 6;

	indices = new int[numIndices];
	int *_indices = indices;
	for (int h = 0; h<nfaces; h++){
		for (int i = 0; i<nfaces; i++){
			int i_plus = i + 1;
			if (i_plus >= nfaces) i_plus = 0;

			int h_plus = h + 1;
			if (h_plus >= nfaces) h_plus = 0;

			*(_indices++) = i + h*nfaces;
			*(_indices++) = i + h_plus*nfaces;
			*(_indices++) = i_plus + h_plus*nfaces;


			*(_indices++) = i_plus + h*nfaces;
			*(_indices++) = i + h*nfaces;

			if (i_plus + h_plus*nfaces >= nfaces*nfaces){
				*(_indices++) = 0;
			}
			else{
				*(_indices++) = i_plus + h_plus*nfaces;
			}
		}
	}
}



void LightSphere::draw(int num, float* positions, float* colors){
	if (num == 0) return;

	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, num * 4 * sizeof(float), positions, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buf[3]);
	glBufferData(GL_ARRAY_BUFFER, num * 4 * sizeof(float), colors, GL_DYNAMIC_DRAW);

	glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices, num);

	glBindVertexArray(0);
}

int DeferredUtils::numLights = 0;
float* DeferredUtils::positions = new float[MAX_NUM_LIGHTS * 4];
float* DeferredUtils::colors = new float[MAX_NUM_LIGHTS * 4];
LightSphere* DeferredUtils::sphere = new LightSphere();

GlobalLight DeferredUtils::globalLight = GlobalLight::DEFUALT;

ScreenQuad* DeferredUtils::screenQuad = new ScreenQuad();

void DeferredUtils::init(float genRadius, int genVertices){
	sphere->gen(genRadius, genVertices);

	screenQuad->gen();
}

void DeferredUtils::setGlobalLight(GlobalLight globalLight){
	DeferredUtils::globalLight = globalLight;
}

void DeferredUtils::addLight(Light& light){
	if (numLights >= MAX_NUM_LIGHTS){
		printf("Max number of lights hit!\n");
	}

	positions[numLights * 4 + 0] = light.pos.x;
	positions[numLights * 4 + 1] = light.pos.y;
	positions[numLights * 4 + 2] = light.pos.z;
	positions[numLights * 4 + 3] = light.radius;

	colors[numLights * 4 + 0] = light.color.r;
	colors[numLights * 4 + 1] = light.color.g;
	colors[numLights * 4 + 2] = light.color.b;
	colors[numLights * 4 + 3] = light.color.a;

	numLights++;
}

void DeferredUtils::applyGlobalLighting(GLTexture& ambient, GLTexture& diffuse, GLTexture& normals){
	GLProgram* shader = Shaders::getDeferredGlobalShader();
	shader->bind();

	shader->setUniformMat4f("vTransf", GsMat().e);
	shader->setUniformMat4f("vProj", GsMat().e);

	ambient.bind(GL_TEXTURE0);
	diffuse.bind(GL_TEXTURE1);
	normals.bind(GL_TEXTURE2);

	shader->setUniform1i("m_ambient", 0);
	shader->setUniform1i("m_diffuse", 1);
	shader->setUniform1i("m_normals", 2);

	shader->setUniform4f("c_ambient", globalLight.ambient.e);
	shader->setUniform4f("c_diffuse", globalLight.diffuse.e);
	shader->setUniform3f("dir", globalLight.dir.e);

	screenQuad->draw();
	shader->unbind();
}

void DeferredUtils::applyLighting(GLTexture& diffuse, GLTexture& normals, GLTexture& depth, const GsMat& proj){
	GLProgram* shader = Shaders::getDeferredLightShader();
	shader->bind();

	shader->setUniformMat4f("vProj", proj.e);

	GsMat projInv = proj;
	projInv.invert();
	shader->setUniformMat4f("vProjInv", projInv.e);

	diffuse.bind(GL_TEXTURE0);
	normals.bind(GL_TEXTURE1);
	depth.bind(GL_TEXTURE2);
	shader->setUniform1i("m_diffuse", 0);
	shader->setUniform1i("m_normals", 1);
	shader->setUniform1i("m_depth", 2);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	sphere->draw(numLights, positions, colors);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->unbind();

	numLights = 0;
}

void DeferredUtils::drawTextureToDepth(GLTexture& tex){
	glDepthMask(GL_TRUE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//glEnable(GL_DEPTH_TEST); // just make sure depth is enabled

	// DRAW
	GLProgram* shader = Shaders::getDepthShader();
	shader->bind();

	shader->setUniformMat4f("vTransf", GsMat().e);
	shader->setUniformMat4f("vProj", GsMat().e);

	tex.bind(GL_TEXTURE0);

	shader->setUniform1i("m_depth", 0);

	screenQuad->draw();
	shader->unbind();
	// ^^^

	glDepthMask(GL_FALSE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}