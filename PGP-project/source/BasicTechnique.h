#ifndef BASIC_TECHNIQUE_H
#define BASIC_TECHNIQUE_H

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "Model.h"

class BasicTechnique
{
protected:
	GLuint vao;
	int indices; //amount of indices in vao
	GLuint program;
	GLuint mvpUniform;
	GLuint mvpDepthUniform;
	GLuint mUniform;
	GLuint ti_mUniform;
	GLuint viewPosUniform;
	GLuint lightPosUniform;
	GLuint ambientLightUniform;
	GLuint texDifSamplerUniform;
	GLuint texDepthSamplerUniform;
	GLuint texDif;
	GLuint texDepth;
	glm::mat4 p;
	glm::mat4 m;
	glm::mat4 v;
	glm::mat4 ti_m;
	glm::mat4 mvpDepth;
	glm::vec3 viewPos;
	glm::vec3 lightPos;
	glm::vec3 ambientLight;
	int texDifSampler;
	int texDepthSampler;
	GLuint drawMode;
public:
	void init(Mesh &m, GLuint p);
	virtual void draw();
	void setM(const glm::mat4 &mat);
	void setV(const glm::mat4 &mat);
	void setP(const glm::mat4 &mat);
	void setViewPos(const glm::vec3 &v);
	void setLightPos(const glm::vec3 &p);
	void setAmbientLight(const glm::vec3 &a);
	void setDepth(const glm::mat4 &mvp, GLuint texture);
	void bindTexDif(int t);
	void bindTexDepth(int t);
};

#endif //BASIC_TECHNIQUE_H