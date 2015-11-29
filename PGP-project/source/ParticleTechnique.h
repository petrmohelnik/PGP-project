#ifndef PARTICLE_TECHNIQUE_H
#define PARTICLE_TECHNIQUE_H

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "Model.h"

struct ParticlePool
{
	glm::vec4 pos; //w = ttl
	ParticlePool(glm::vec4 p) : pos(p) {}
};

struct SortList
{
	GLuint index;
	float distance;
	SortList(GLuint i, float d) : index(i), distance(d) {}
};

struct DeadList
{
	GLuint index;
	DeadList(GLuint i) : index(i) {}
};

class ParticleTechnique
{
protected:
	GLuint vao;
	GLuint vbo[5]; //pool, sortList, deadList, sortCounter, deadCounter
	int indices; //amount of indices in vao
	GLuint program;
	GLuint simulateComputeProgram;
	GLuint emitComputeProgram;
	GLuint sortComputeProgram;
	GLuint mvUniform;
	GLuint pUniform;
	GLuint dtUniform;
	GLuint viewPosUniform;
	GLuint maxParticlesUniform;
	GLuint maxEmitUniform;
	GLuint texDifSamplerUniform;
	GLuint texDif;
	glm::mat4 p;
	glm::mat4 m;
	glm::mat4 v;
	int dt;
	glm::vec3 viewPos;
	int texDifSampler;
public:
	void init(Mesh &m, int count, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP);
	void draw();
	void setM(glm::mat4 mat);
	void setV(glm::mat4 mat);
	void setP(glm::mat4 mat);
	void setDt(int t);
	void setViewPos(glm::vec3 pos);
	void bindTexDif(int t);
};

#endif //PARTICLE_TECHNIQUE_H