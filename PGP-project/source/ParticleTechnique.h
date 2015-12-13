#ifndef PARTICLE_TECHNIQUE_H
#define PARTICLE_TECHNIQUE_H

#define GRID_SIZE 6.0
#define GRID_H 0.10
#define MASS 0.00005
#define GAS_CONSTANT 4
#define REST_DENSITY 0.59

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <math.h> 
#include "Model.h"

struct ParticlePool
{
	glm::vec4 pos; //w = ttl
	glm::vec4 velocity; //w=density
	glm::vec4 force; //w=pressure
	ParticlePool(glm::vec4 p, glm::vec4 v, glm::vec4 f) : pos(p), velocity(v), force(f) {}
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

struct GridList {
	GLuint particle_id;
	float cell_id;
	GridList(float c, GLuint p) : cell_id(c), particle_id(p) {}
};

struct StartIndexList {
	GLuint start;
	StartIndexList(GLuint s) : start(s) {}
};

class ParticleTechnique
{
protected:
	GLuint vao;
	GLuint vbo[8]; //pool, sortList, deadList, sortCounter, deadCounter, gridList, startIndexList, gridCounter
	int indices; //amount of indices in vao
	GLuint program;
	GLuint simulateComputeProgram;
	GLuint emitComputeProgram;
	GLuint sortComputeProgram;
	GLuint sortLocalComputeProgram;
	GLuint sortLocalInnerComputeProgram;
	GLuint gridDivideComputeProgram;
	GLuint gridFindStartComputeProgram;
	GLuint simulateDensityComputeProgram;
	GLuint simulatePressureComputeProgram;
	GLuint simulateForceComputeProgram;
	GLuint mvUniform;
	GLuint pUniform;
	GLuint dtUniform;
	GLuint halfVectorUniform;
	GLuint maxParticlesUniform;
	GLuint hGridSimulateUniform;
	GLuint gridMaxIndexUniform;
	GLuint gridSizeSimulateUniform;
	GLuint maxEmitUniform;
	GLuint maxSortUniform;
	GLuint maxSortLocalUniform;
	GLuint degreeSortUniform;
	GLuint maxSortLocalInnerUniform;
	GLuint compareDistUniform;
	GLuint subArraySizeUniform;
	GLuint subArraySizeLocalInnerUniform;
	GLuint texDifSamplerUniform;
	GLuint maxParticlesGridDivideUniform;
	GLuint sizeGridDivideUniform;
	GLuint hGridDivideUniform;
	GLuint maxParticlesDensityUniform;
	GLuint hGridSimulateDensityUniform;
	GLuint gridMaxIndexDensityUniform;
	GLuint gridSizeSimulateDensityUniform;
	GLuint massSimulateDensityUniform;
	GLuint gasConstantPressureUniform;
	GLuint restDensityPressureUniform;
	GLuint maxParticlesForceUniform;
	GLuint hGridSimulateForceUniform;
	GLuint gridMaxIndexForceUniform;
	GLuint gridSizeSimulateForceUniform;
	GLuint massSimulateForceUniform;
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
	void init(Mesh &m, int count, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP, GLuint sortLocalComputeP, 
		GLuint sortLocalInnerComputeP, GLuint gridDivideComputeP, GLuint gridFindStartComputeP,
		GLuint simulateDensityComputeP, GLuint simulatePressureComputeP, GLuint simulateForceComputeP);
	void draw();
	void sort(GLuint sortCounter, GLuint buffer);
	void setM(glm::mat4 mat);
	void setV(glm::mat4 mat);
	void setP(glm::mat4 mat);
	void setDt(int t);
	void setViewPos(glm::vec3 pos);
	void setHalfVector(glm::vec3 halfVec, bool flip);
	void bindTexDif(int t);
};

#endif //PARTICLE_TECHNIQUE_H