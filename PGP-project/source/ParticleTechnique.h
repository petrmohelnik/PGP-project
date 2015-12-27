#ifndef PARTICLE_TECHNIQUE_H
#define PARTICLE_TECHNIQUE_H

#define GRID_SIZE 4.0
#define GRID_H 0.06
#define MASS 0.00005
#define GAS_CONSTANT 4.0
#define REST_DENSITY 0.59
#define VISCOSITY 0.01
#define BUOYANCY 5.0
#define GRAVITY -9.81

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <math.h> 
#include "Model.h"

struct ParticlePool
{
	glm::vec4 pos; //w = ttl
	glm::vec4 velocity; //w=density
	glm::vec4 force; //w=pressure
	glm::vec4 velocity_prev; //v case -deltaT/2 - pouziti pro leap frog integrator
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
	GLuint vbo[9]; //pool, sortList, deadList, sortCounter, deadCounter, gridList, startIndexList, gridCounter, sortedParticlePool
	int indices; //amount of indices in vao
	GLuint program;
	GLuint simulateComputeProgram;
	GLuint emitComputeProgram;
	GLuint sortPreComputeProgram;
	GLuint sortComputeProgram;
	GLuint sortLocalComputeProgram;
	GLuint sortLocalInnerComputeProgram;
	GLuint gridDivideComputeProgram;
	GLuint gridFindStartComputeProgram;
	GLuint simulateDensityComputeProgram;
	GLuint simulatePressureComputeProgram;
	GLuint simulateForceComputeProgram;
	GLuint mvUniform;
	GLuint mvpDepthUniform;
	GLuint mvpDepth2Uniform;
	GLuint pUniform;
	GLuint dtUniform;
	GLuint halfVectorUniform;
	GLuint maxParticlesUniform;
	GLuint hGridSimulateUniform;
	GLuint gridMaxIndexUniform;
	GLuint gridSizeSimulateUniform;
	GLuint buoyancySimulateUniform;
	GLuint restDensitySimulateUniform;
	GLuint gravitySimulateUniform;
	GLuint timeSimulateUniform;
	GLuint maxEmitUniform;
	GLuint maxParticlesSortPreUniform;
	GLuint maxSortUniform;
	GLuint maxSortLocalUniform;
	GLuint degreeSortUniform;
	GLuint maxSortLocalInnerUniform;
	GLuint compareDistUniform;
	GLuint subArraySizeUniform;
	GLuint subArraySizeLocalInnerUniform;
	GLuint ambientLightUniform;
	GLuint texDifSamplerUniform;
	GLuint texDepthSamplerUniform;
	GLuint texDepth2SamplerUniform;
	GLuint texDepth3SamplerUniform;
	GLuint maxParticlesGridDivideUniform;
	GLuint sizeGridDivideUniform;
	GLuint hGridDivideUniform;
	GLuint maxParticlesDensityUniform;
	GLuint hGridSimulateDensityUniform;
	GLuint gridMaxIndexDensityUniform;
	GLuint gridSizeSimulateDensityUniform;
	GLuint massSimulateDensityUniform;
	GLuint maxParticlesPressureUniform;
	GLuint gasConstantPressureUniform;
	GLuint restDensityPressureUniform;
	GLuint maxParticlesForceUniform;
	GLuint hGridSimulateForceUniform;
	GLuint gridMaxIndexForceUniform;
	GLuint gridSizeSimulateForceUniform;
	GLuint massSimulateForceUniform;
	GLuint viscositySimulateForceUniform;
	GLuint texDif;
	GLuint texDepth;
	GLuint texDepth2;
	GLuint texDepth3;
	GLuint sortCounter;
	glm::mat4 p;
	glm::mat4 m;
	glm::mat4 v;
	glm::mat4 mvpDepth;
	glm::mat4 mvpDepth2;
	glm::vec3 halfVector;
	bool flippedHalfVector;
	int dt;
	glm::vec3 viewPos;
	glm::vec3 ambientLight;
	int texDifSampler;
	int texDepthSampler;
	int texDepth2Sampler;
	int texDepth3Sampler;
	GLuint gridCounter;
	int counter = 0;
	float time = 0;
public:
	void init(Mesh &m, int count, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortPreComputeP, GLuint sortComputeP,
		GLuint sortLocalComputeP, GLuint sortLocalInnerComputeP, GLuint gridDivideComputeP, GLuint gridFindStartComputeP,
		GLuint simulateDensityComputeP, GLuint simulatePressureComputeP, GLuint simulateForceComputeP);
	void simulate();
	void draw();
	void sort(GLuint sortCounter, GLuint buffer);
	void sortParticles();
	void setM(const glm::mat4 &mat);
	void setV(const glm::mat4 &mat);
	void setP(const glm::mat4 &mat);
	void setDt(int t);
	void setViewPos(const glm::vec3 &pos);
	void setHalfVector(const glm::vec3 &halfVec, bool flip);
	void setAmbientLight(const glm::vec3 &a);
	void setDepth(const glm::mat4 &mvp, const glm::mat4 &mvp2, GLuint texture, GLuint texture2, GLuint texture3);
	void bindTexDif(int t);
	void bindTexDepth(int t, int t2, int t3);
};

#endif //PARTICLE_TECHNIQUE_H