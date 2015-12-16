#include "ParticleSystemRenderer.h"

ParticleSystemRenderer::ParticleSystemRenderer(glm::vec3 position)
{
	pos = position;
	technique.reset(new ParticleTechnique);
}

bool ParticleSystemRenderer::initRenderer(Model &m, int count, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP,
	GLuint sortLocalComputeP, GLuint sortLocalInnerComputeP, GLuint gridDivideComputeP, GLuint gridFindStartComputeP,
	GLuint simulateDensityComputeP, GLuint simulatePressureComputeP, GLuint simulateForceComputeP)
{
	if (m.getMeshesSize() < 1) {
		std::cout << "ERROR: Model is empty!";
		return false;
	}

	technique->init(*(m.getMeshes()[0]), count, p, simulateComputeP, emitComputeP, sortComputeP, sortLocalComputeP, 
		sortLocalInnerComputeP, gridDivideComputeP, gridFindStartComputeP,
		simulateDensityComputeP, simulatePressureComputeP, simulateForceComputeP);
	return true;
}

void ParticleSystemRenderer::render(Camera &cam, std::vector<Light> &lights, glm::vec3 ambientLight, int dt, DrawType drawType)
{
	technique->setP(cam.getProjection());
	technique->setV(cam.getView());
	technique->setViewPos(cam.getPos());
	technique->bindTexDif(0);

	glm::mat4 M = glm::mat4(1.0);
	M = glm::translate(M, pos);

	technique->setM(M);

	technique->draw();
}

void ParticleSystemRenderer::simulate(Camera &cam, std::vector<Light> &lights, int dt)
{
	technique->setDt(dt);

	glm::vec3 halfVector;
	bool flipped;

	if (glm::dot(cam.getDir(), lights[0].dir) > 0.0) {
		halfVector = glm::normalize(cam.getDir() + lights[0].dir);
		flipped = false;
	}
	else {
		halfVector = glm::normalize(-cam.getDir() + lights[0].dir);
		flipped = true;
	}

	technique->setHalfVector(halfVector, flipped);
	technique->simulate();
}

