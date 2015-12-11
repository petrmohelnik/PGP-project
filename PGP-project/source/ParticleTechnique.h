#ifndef PARTICLE_TECHNIQUE_H
#define PARTICLE_TECHNIQUE_H

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <math.h> 
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
	GLuint sortLocalComputeProgram;
	GLuint sortLocalInnerComputeProgram;
	GLuint mvUniform;
	GLuint pUniform;
	GLuint dtUniform;
	GLuint halfVectorUniform;
	GLuint maxParticlesUniform;
	GLuint maxEmitUniform;
	GLuint maxSortUniform;
	GLuint maxSortLocalUniform;
	GLuint degreeSortUniform;
	GLuint maxSortLocalInnerUniform;
	GLuint compareDistUniform;
	GLuint subArraySizeUniform;
	GLuint subArraySizeLocalInnerUniform;
	GLuint texDifSamplerUniform;
	GLuint texDif;
	glm::mat4 p;
	glm::mat4 m;
	glm::mat4 v;
	glm::vec3 halfVector;
	bool flippedHalfVector;
	int dt;
	glm::vec3 viewPos;
	int texDifSampler;
public:
	void init(Mesh &m, int count, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP, GLuint sortLocalComputeP, GLuint sortLocalInnerComputeP);
	void draw();
	void setM(glm::mat4 mat);
	void setV(glm::mat4 mat);
	void setP(glm::mat4 mat);
	void setDt(int t);
	void setViewPos(glm::vec3 pos);
	void setHalfVector(glm::vec3 halfVec, bool flip);
	void bindTexDif(int t);
};

#endif //PARTICLE_TECHNIQUE_H