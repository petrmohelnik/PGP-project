#include "ParticleSystemRenderer.h"

ParticleSystemRenderer::ParticleSystemRenderer(const glm::vec3 &position)
{
	pos = position;
	technique.reset(new ParticleTechnique);
}

bool ParticleSystemRenderer::initRenderer(Model &m, int count, GLuint p, GLuint pShaft, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortPreComputeP, GLuint sortComputeP,
	GLuint sortLocalComputeP, GLuint sortLocalInnerComputeP, GLuint gridDivideComputeP, GLuint gridFindStartComputeP,
	GLuint simulateDensityComputeP, GLuint simulatePressureComputeP, GLuint simulateForceComputeP)
{
	if (m.getMeshesSize() < 1) {
		std::cout << "ERROR: Model is empty!";
		return false;
	}

	technique->init(*(m.getMeshes()[0]), count, p, pShaft, simulateComputeP, emitComputeP, sortPreComputeP, sortComputeP, sortLocalComputeP,
		sortLocalInnerComputeP, gridDivideComputeP, gridFindStartComputeP,
		simulateDensityComputeP, simulatePressureComputeP, simulateForceComputeP);
	return true;
}

void ParticleSystemRenderer::render(Camera &cam, const std::vector<Light> &lights, const glm::vec3 &ambientLight, const glm::mat4 &mvpDepth, const glm::mat4 &mvpDepth2, GLuint texDepth, GLuint texDepth2, GLuint texDepth3, GLuint texDepth4, int dt, DrawType drawType)
{
	technique->setAmbientLight(ambientLight + technique->getMesh()->getMaterial()->getEmission());
	technique->setP(cam.getProjection());
	technique->setV(cam.getView());
	technique->setViewPos(cam.getPos());

	glm::mat4 M = glm::mat4(1.0);
	M = glm::translate(M, pos);

	technique->setM(M);

	const glm::mat4 bias(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);

	technique->setDepth(bias * mvpDepth * M, bias * mvpDepth2 * M, texDepth, texDepth2, texDepth3, texDepth4);
	technique->bindTexDepth(1, 2, 3, 4);
	technique->bindTexDif(0);

	technique->draw();
}

void ParticleSystemRenderer::renderShafts(Camera &cam, int dt)
{
	technique->setAmbientLight(technique->getMesh()->getMaterial()->getEmission());
	technique->setP(cam.getProjection());
	technique->setV(cam.getView());
	technique->setViewPos(cam.getPos());

	glm::mat4 M = glm::mat4(1.0);
	M = glm::translate(M, pos);

	technique->setM(M);
	technique->bindTexDif(0);

	technique->drawShafts();
}

void ParticleSystemRenderer::simulate(int dt)
{
	technique->setDt(dt);

	technique->simulate();
}

void ParticleSystemRenderer::sort(Camera &cam, const std::vector<Light> &lights)
{
	glm::vec3 halfVector;
	bool flipped;

	if (glm::dot(cam.getDir(), lights[0].dir) > 0.0) {
		//halfVector = glm::normalize(cam.getDir() + lights[0].dir);
		flipped = false;
	}
	else {
		//halfVector = glm::normalize(-cam.getDir() + lights[0].dir);
		flipped = true;
	}
	halfVector = -cam.getDir();

	technique->setHalfVector(halfVector, flipped);
	technique->sortParticles();
}

void *ParticleSystemRenderer::getTechnique()
{
	return technique.get();
}