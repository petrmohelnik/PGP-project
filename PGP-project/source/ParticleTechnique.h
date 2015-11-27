#ifndef PARTICLE_TECHNIQUE_H
#define PARTICLE_TECHNIQUE_H

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "Model.h"

struct SortList
{
	GLuint index;
	float distance;
	SortList(GLuint i, float d) : index(i), distance(d) {}
};

class ParticleTechnique
{
protected:
	GLuint vao;
	GLuint vbo[3];
	int indices; //amount of indices in vao
	GLuint program;
	GLuint simulateComputeProgram;
	GLuint emitComputeProgram;
	GLuint sortComputeProgram;
	GLuint mvUniform;
	GLuint pUniform;
	GLuint texDifSamplerUniform;
	GLuint texDif;
	glm::mat4 p;
	glm::mat4 m;
	glm::mat4 v;
	int texDifSampler;
public:
	void init(Mesh &m, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP);
	void draw();
	void setM(glm::mat4 mat);
	void setV(glm::mat4 mat);
	void setP(glm::mat4 mat);
	void bindTexDif(int t);
};

#endif //PARTICLE_TECHNIQUE_H