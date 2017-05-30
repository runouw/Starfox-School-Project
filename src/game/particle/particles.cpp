#include "particles.h"

#include <rtools\ai\textures.h>
#include <rtools\ai\shaders.h>
#include <game\engine.h>

float* ParticleUtils::positions = new float[MAX_PARTICLES * 4];
float* ParticleUtils::colors = new float[MAX_PARTICLES * 4];

void ParticleQuad::gen(){
	numVertices = 4;

	va = 0;
	buf = 0;
	gen_vertex_arrays(1); // sphere's vba
	gen_buffers(4); // x,y,z; uvx, uvy, xyzr, rgba; 

	float* _p = new float[numVertices * 3]; // positions
	float* p = _p;

	float* _uvs = new float[numVertices * 2]; // uvs
	float* uvs = _uvs;

	*(p++) = -.5f; *(p++) = -.5f; *(p++) = 0;
	*(p++) = .5f; *(p++) = -.5f; *(p++) = 0;
	*(p++) = .5f; *(p++) = .5f; *(p++) = 0;
	*(p++) = -.5f; *(p++) = .5f; *(p++) = 0;

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

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // uvs
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // x,y,z, scale
	//glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, buf[3]); // r, g, b, a
	//glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);

}
void ParticleQuad::draw(int num, float* positions, float* colors){
	if (num == 0) return;

	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, num * 4 * sizeof(float), positions, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buf[3]);
	glBufferData(GL_ARRAY_BUFFER, num * 4 * sizeof(float), colors, GL_DYNAMIC_DRAW);
	
	glDrawArraysInstanced(GL_QUADS, 0, numVertices, num);

	glBindVertexArray(0);
}


ParticleQuad ParticleUtils::quad;

void ParticleUtils::init(){
	quad = ParticleQuad();
	quad.gen();
}

void ParticleUtils::addParticle(Particles &particles, Particle* particle){
	std::vector<Particle*>* list = getParticleList(particles, particle->tex->texId);
	
	list->push_back(particle);
}

std::vector<Particle*>* ParticleUtils::getParticleList(Particles &particles, int texID){
	if (particles.map.find(texID) == particles.map.end()){
		particles.map[texID] = new std::vector<Particle*>();
		particles.map[texID]->reserve(MAX_PARTICLES);
	}

	return particles.map[texID];
}


void ParticleUtils::idle(Particles &particles, float dt){
	std::map<int, std::vector<Particle*>*>::iterator itr = particles.map.begin();

	while (itr != particles.map.end()){
		std::vector<Particle*>* list = itr->second;

		for (size_t i = 0; i < list->size(); i++){
			Particle* p = (*list)[i];
			if (p->exists){
				p->idle(dt);
			}else{
				// removes
				if ((*list)[i]->del){
					delete (*list)[i];
				}
				
				(*list)[i] = (*list)[list->size() - 1];
				list->pop_back();
				i--;
			}

		}
		++itr;
	}
}

void ParticleUtils::draw(Particles &particles, const GsMat& cam, const GsMat& proj){
	glDepthMask(GL_FALSE);
	GLProgram* program = Shaders::getParticleShader();

	program->bind();

	program->setUniformMat4f("vCam", (cam).e);
	program->setUniformMat4f("vProj", (proj).e);

	program->setUniform1i("Texture0", 0);
	glActiveTexture(GL_TEXTURE0);

	std::map<int, std::vector<Particle*>*>::iterator itr = particles.map.begin();
	while (itr != particles.map.end()){
		std::vector<Particle*>*list = itr->second;


		int num = 0;
		for (size_t i = 0; i < list->size(); i++){
			if (i > MAX_PARTICLES){
				printf("Max number of particles reached!\n");
				break;
			}
			Particle* p = (*list)[i];

			positions[i * 4 + 0] = p->pos.x;
			positions[i * 4 + 1] = p->pos.y;
			positions[i * 4 + 2] = p->pos.z;
			positions[i * 4 + 3] = p->size;

			colors[i * 4 + 0] = p->color.r * p->color.a;
			colors[i * 4 + 1] = p->color.g * p->color.a;
			colors[i * 4 + 2] = p->color.b * p->color.a;
			colors[i * 4 + 3] = p->rotation;

			num++;
		}

		// Draw this batch:
		
		glBindTexture(GL_TEXTURE_2D, itr->first);

		quad.draw(num, positions, colors);

		++itr;
	}
	program->unbind();
	glDepthMask(GL_TRUE);
}

Particle::Particle(GLTexture* tex, GsVec pos, float size, RColor color){
	this->tex = tex;
	this->pos = pos;
	this->size = size;
	this->color = color;
}

Particle_Simple::Particle_Simple(GLTexture* tex, GsVec pos, float size, RColor color, GsVec vel, float duration){
	this->tex = tex;
	this->pos = pos;
	this->size = size;
	this->color = color;
	this->vel = vel;
	this->duration = duration;
	t = 0;

	exists = true;
}
void Particle_Simple::idle(float dt){
	pos += vel * dt;
	t += dt;

	if (t > duration){
		exists = false;
	}
}

Particle_SimpleDecay::Particle_SimpleDecay(GLTexture* tex, GsVec pos, float size, RColor color, GsVec vel, float duration){
	this->tex = tex;
	this->pos = pos;
	this->size = size;
	this->color = color;
	this->vel = vel;
	this->duration = duration;
	t = 0;

	initialSize = this->size;

	exists = true;
}
void Particle_SimpleDecay::idle(float dt){
	pos += vel * dt;
	t += dt;

	float decay = (duration - t) / duration;
	decay = min(decay * decay, 1.0f);

	size = decay * initialSize;

	if (t > duration){
		exists = false;
	}
}


Particle_ExplosionDecay::Particle_ExplosionDecay(GLTexture* tex, GsVec pos, float size, RColor color, GsVec vel, float duration, float rotSpeed){
	this->tex = tex;
	this->pos = pos;
	this->size = size;
	this->color = color;
	this->vel = vel;
	this->duration = duration;
	this->rotSpeed = rotSpeed;
	t = 0;

	initialSize = this->size;

	exists = true;
}
void Particle_ExplosionDecay::idle(float dt){
	pos += vel * dt;
	t += dt;
	rotation += rotSpeed * dt;

	float decay = (duration - t) / duration;
	decay = min(decay * decay, 1.0f);

	color.a = decay;

	size = (1-decay) * initialSize;

	if (t > duration){
		exists = false;
	}
}
