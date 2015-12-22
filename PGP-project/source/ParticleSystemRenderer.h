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
	bool initRenderer(Model &m, int count, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP,
		GLuint sortLocalComputeP, GLuint sortLocalInnerComputeP, GLuint gridDivideComputeP, GLuint gridFindStartComputeP,
		GLuint simulateDensityComputeP, GLuint simulatePressureComputeP, GLuint simulateForceComputeP);
	void render(Camera &cam, const std::vector<Light> &lights, const glm::vec3 &ambientLight, const glm::mat4 &mvpDepth, GLuint texDepth, int dt, DrawType drawType);
	void simulate(Camera &cam, const std::vector<Light> &lights, int dt);
};

#endif //PARTICLE_SYSTEM_RENDERER_H