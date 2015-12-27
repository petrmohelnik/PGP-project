#ifndef PARTICLE_SYSTEM_RENDERER_H
#define PARTICLE_SYSTEM_RENDERER_H

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "ParticleTechnique.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"

class ParticleSystemRenderer : public Renderer
{
private:
	std::shared_ptr<ParticleTechnique> technique;
	glm::vec3 pos;
public:
	ParticleSystemRenderer(const glm::vec3 &position);
	bool initRenderer(Model &m, int count, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortPreComputeP, GLuint sortComputeP,
		GLuint sortLocalComputeP, GLuint sortLocalInnerComputeP, GLuint gridDivideComputeP, GLuint gridFindStartComputeP,
		GLuint simulateDensityComputeP, GLuint simulatePressureComputeP, GLuint simulateForceComputeP);
	void render(Camera &cam, const std::vector<Light> &lights, const glm::vec3 &ambientLight, const glm::mat4 &mvpDepth, const glm::mat4 &mvpDepth2, GLuint texDepth, GLuint texDepth2, GLuint texDepth3, int dt, DrawType drawType);
	void simulate(int dt);
	void sort(Camera &cam, const std::vector<Light> &lights);
};

#endif //PARTICLE_SYSTEM_RENDERER_H