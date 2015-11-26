#ifndef PARTICLE_TECHNIQUE_H
#define PARTICLE_TECHNIQUE_H

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "Model.h"

class ParticleTechnique
{
protected:
	GLuint vao;
	int indices; //amount of indices in vao
	GLuint program;
	GLuint mvUniform;
	GLuint pUniform;
	GLuint texDifSamplerUniform;
	GLuint texDif;
	glm::mat4 p;
	glm::mat4 m;
	glm::mat4 v;
	int texDifSampler;
public:
	void init(Mesh &m, GLuint p);
	void draw();
	void setM(glm::mat4 mat);
	void setV(glm::mat4 mat);
	void setP(glm::mat4 mat);
	void bindTexDif(int t);
};

#endif //PARTICLE_TECHNIQUE_H