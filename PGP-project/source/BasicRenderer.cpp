#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(const glm::vec3 &position)
{
	pos = position;
	technique.reset(new BasicTechnique);
}

bool BasicRenderer::initRenderer(Model &m, GLuint p, GLuint pShaft)
{
	if (m.getMeshesSize() < 1) {
		std::cout << "ERROR: Model is empty!";
			return false;
	}

	technique->init(*(m.getMeshes()[0]), p, pShaft);
	return true;
}

void BasicRenderer::render(Camera &cam, const std::vector<Light> &lights, const glm::vec3 &ambientLight, const glm::mat4 &mvpDepth, const glm::mat4 &mvpDepth2, GLuint texDepth, GLuint texDepth2, GLuint texDepth3, GLuint texDepth4, int dt, DrawType drawType)
{
	if (lights.size() > 0)
		technique->setLightPos(lights[0].pos);
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


	/*M = glm::translate(M, glm::vec3(0.25));
	M = glm::scale(M, glm::vec3(0.7));
	technique->setM(M);

	technique->draw();

	M = glm::translate(M, glm::vec3(0.25));
	M = glm::scale(M, glm::vec3(0.7));
	technique->setM(M);

	technique->draw();*/
}

void BasicRenderer::renderShafts(Camera &cam, int dt)
{
	technique->setAmbientLight(technique->getMesh()->getMaterial()->getEmission());
	technique->setP(cam.getProjection());
	technique->setV(cam.getView());
	technique->setViewPos(cam.getPos());

	glm::mat4 M = glm::mat4(1.0);
	M = glm::translate(M, pos);

	technique->setM(M);
	//technique->bindTexDif(0);

	technique->drawShafts();
}

void *BasicRenderer::getTechnique()
{
	return technique.get();
}