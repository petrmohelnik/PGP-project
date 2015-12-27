#ifndef BASIC_TECHNIQUE_H
#define BASIC_TECHNIQUE_H

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "Model.h"

class BasicTechnique
{
protected:
	GLuint vao;
	GLuint vaoShaft;
	int indices; //amount of indices in vao
	GLuint program;
	GLuint programShaft;
	GLuint mvpUniform;
	GLuint mvpDepthUniform;
	GLuint mvpDepth2Uniform;
	GLuint mUniform;
	GLuint ti_mUniform;
	GLuint viewPosUniform;
	GLuint lightPosUniform;
	GLuint ambientLightUniform;
	GLuint texDifSamplerUniform;
	GLuint texDepthSamplerUniform;
	GLuint texDepth2SamplerUniform;
	GLuint texDepth3SamplerUniform;
	GLuint texDif;
	GLuint texDepth;
	GLuint texDepth2;
	GLuint texDepth3;

	GLuint mvpUniformShaft;
	GLuint ambientLightUniformShaft;

	glm::mat4 p;
	glm::mat4 m;
	glm::mat4 v;
	glm::mat4 ti_m;
	glm::mat4 mvpDepth;
	glm::mat4 mvpDepth2;
	glm::vec3 viewPos;
	glm::vec3 lightPos;
	glm::vec3 ambientLight;
	int texDifSampler;
	int texDepthSampler;
	int texDepth2Sampler;
	int texDepth3Sampler;
	GLuint drawMode;
	Mesh *mesh;

public:
	void init(Mesh &m, GLuint p, GLuint pShaft);
	virtual void draw();
	virtual void drawShafts();
	void setM(const glm::mat4 &mat);
	void setV(const glm::mat4 &mat);
	void setP(const glm::mat4 &mat);
	void setViewPos(const glm::vec3 &v);
	void setLightPos(const glm::vec3 &p);
	void setAmbientLight(const glm::vec3 &a);
	void setDepth(const glm::mat4 &mvp, const glm::mat4 &mvp2, GLuint texture, GLuint texture2, GLuint texture3);
	void bindTexDif(int t);
	void bindTexDepth(int t, int t2, int t3);

	Mesh *getMesh();
};

#endif //BASIC_TECHNIQUE_H