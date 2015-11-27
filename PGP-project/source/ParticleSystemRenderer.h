#ifndef PARTICLE_SYSTEM_RENDERER_H
#define PARTICLE_SYSTEM_RENDERER_H

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "ParticleTechnique.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "Scene.h"

class ParticleSystemRenderer : public Renderer
{
private:
	std::shared_ptr<ParticleTechnique> technique;
	glm::vec3 pos;
public:
	ParticleSystemRenderer(glm::vec3 position);
	bool initRenderer(Model &m, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP);
	void render(Camera &cam, std::vector<Light> &lights, glm::vec3 ambientLight);

};

#endif //PARTICLE_SYSTEM_RENDERER_H