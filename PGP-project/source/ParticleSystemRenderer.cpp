#include "ParticleSystemRenderer.h"

ParticleSystemRenderer::ParticleSystemRenderer(glm::vec3 position)
{
	pos = position;
	technique.reset(new ParticleTechnique);
}

bool ParticleSystemRenderer::initRenderer(Model &m, int count, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP, GLuint sortLocalComputeP, GLuint sortLocalInnerComputeP)
{
	if (m.getMeshesSize() < 1) {
		std::cout << "ERROR: Model is empty!";
		return false;
	}

	technique->init(*(m.getMeshes()[0]), count, p, simulateComputeP, emitComputeP, sortComputeP, sortLocalComputeP, sortLocalInnerComputeP);
	return true;
}

void ParticleSystemRenderer::render(Camera &cam, std::vector<Light> &lights, glm::vec3 ambientLight, int dt)
{
	technique->setP(cam.getProjection());
	technique->setV(cam.getView());
	technique->setDt(dt);
	technique->setViewPos(cam.getPos());
	technique->bindTexDif(0);

	glm::mat4 M = glm::mat4(1.0);
	M = glm::translate(M, pos);

	technique->setM(M);

	technique->draw();
}

